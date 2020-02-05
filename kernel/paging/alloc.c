#include <paging/alloc.h>
#include <paging/conv.h>
#include <stdbool.h>
#include <string.h>
#include <util/algo.h>
#include <util/bitmap.h>
#include <util/list.h>

#define MAX_REGIONS ((size_t)24)

static struct fakix_memmap coalesced_regions[MAX_REGIONS];

/* Populate coalesced_regions with the combined overlapping regions in mmap. */
static void coalesce_regions(struct fakix_memmap *mmap);

struct page_pool {
    struct list_elem master_elem;
    struct bitmap *page_map;
    uintptr_t base;
    size_t length;
};

static void *page_pool_allocate(pal_flags_t al_flags, size_t npages);
static void page_pool_free(void *pagebase, size_t npages);

static struct list page_master_table;

void init_page_alloc(struct fakix_memmap *mmap)
{
    memset(coalesced_regions, 0, sizeof coalesced_regions);
    /* Skip the first entry, which contains sensitive data. */
    coalesce_regions(++mmap);
    list_init(&page_master_table);

    size_t i;
    for (i = 0; i < MAX_REGIONS; ++i) {
        if (coalesced_regions[i].length == 0) {
            break;
        }

        size_t npages = coalesced_regions[i].length / FAKIX_PAGE;
        size_t buffersz = div_rnd_up(npages, 8);
        struct page_pool *pg_pool =
            conv_phys_to_kern(coalesced_regions[i].base_address);
        list_push_back(&page_master_table, &pg_pool->master_elem);
        npages = npages - buffersz - 1;
        pg_pool->page_map = bitmap_create_buffer(npages, pg_pool + 1, buffersz);
        pg_pool->base = coalesced_regions[i].base_address;
        pg_pool->length = coalesced_regions[i].length;
    }
}

void *page_allocate_multiple(pal_flags_t al_flags, size_t npages)
{
    size_t pageratio = 0;
    void *page = NULL;
    int pal_size = (al_flags & PAGE_ALLOC_SIZEMASK);
    if (pal_size == PAGE_ALLOC_SIZE1G) {
        pageratio = FAKIX_HUGEPAGE / FAKIX_PAGE;
    } else if (pal_size == PAGE_ALLOC_SIZE2M) {
        pageratio = FAKIX_BIGPAGE / FAKIX_PAGE;
    } else if (pal_size == PAGE_ALLOC_SIZE4K) {
        pageratio = FAKIX_PAGE / FAKIX_PAGE;
    }

    if (pageratio != 0 && mul_safe(npages, pageratio, &npages)) {
        page = page_pool_allocate(al_flags, npages);
    }

    return page;
}

void *page_allocate(pal_flags_t al_flags)
{
    return page_allocate_multiple(al_flags, 1);
}

void page_free_multiple(void *pagebase, int size, size_t npages)
{
    size_t pageratio = 0;
    if (size == PAGE_ALLOC_SIZE1G) {
        pageratio = FAKIX_HUGEPAGE / FAKIX_PAGE;
    } else if (size == PAGE_ALLOC_SIZE2M) {
        pageratio = FAKIX_BIGPAGE / FAKIX_PAGE;
    } else if (size == PAGE_ALLOC_SIZE4K) {
        pageratio = FAKIX_PAGE / FAKIX_PAGE;
    }

    if (pageratio != 0 && mul_safe(npages, pageratio, &npages)) {
        page_pool_free(pagebase, npages);
    }
}

void page_free(void *page, int size)
{
    page_free_multiple(page, size, 1);
}

static bool overlapping_regions(struct fakix_memmap *map1,
                                struct fakix_memmap *map2)
{
    return ((map1->base_address < map2->base_address + map2->length) ||
            (map2->base_address < map1->base_address + map1->length));
}

static size_t region_diff(struct fakix_memmap *map1, struct fakix_memmap *map2)
{
    size_t diff;
    diff = (map1->base_address + map1->length);
    diff = diff - (map2->base_address + map2->length);
    diff = (diff & INT64_MIN) ? (~diff + 1) : (diff);
    return diff;
}

/* Populate coalesced_regions with the combined overlapping regions in mmap. */
static void coalesce_regions(struct fakix_memmap *mmap)
{
    size_t i;
    size_t lim = MAX_REGIONS;
    for (i = 0; i < lim; ++i) {
        if (mmap[i].length == 0) {
            lim = i;
        }
    }

    struct fakix_memmap *newmmap = coalesced_regions;
    for (i = 0; i < lim; ++i) {
        if (mmap[i].length == -1) {
            continue;
        }

        if (mmap[i].type == 1) {
            memcpy(newmmap, &mmap[i], sizeof *newmmap);
            size_t j;
            for (j = i + 1; j < lim; ++j) {
                if (mmap[j].length == -1) {
                    continue;
                }

                if (mmap[j].type == 1 &&
                    overlapping_regions(newmmap, mmap + j)) {
                    newmmap->length += region_diff(newmmap, &mmap[j]);
                    if (newmmap->base_address > mmap[j].base_address) {
                        newmmap->base_address = mmap[j].base_address;
                    }
                    memset(mmap + j, -1, sizeof *mmap);
                }
            }
            newmmap++;
        }
    }
}

static void *page_pool_allocate(pal_flags_t al_flags, size_t npages)
{
    void *page = NULL;
    struct list *master = &page_master_table;
    struct list_elem *el;
    for (el = list_begin(master); el != list_end(master); el = list_next(el)) {
        struct page_pool *pool = elem_value(el, struct page_pool, master_elem);
        size_t pageno = bitmap_scan_and_flip(pool->page_map, 0, npages, 0);

        if (pageno != BITMAP_NPOS) {
            page = conv_phys_to_kern((pageno * FAKIX_PAGE) + pool->base);
            if (al_flags & PAGE_ALLOC_ZERO) {
                memset(page, 0, FAKIX_PAGE);
            }
            break;
        }
    }

    if (al_flags & PAGE_ALLOC_ASSERT) {
        /* Assert page != NULL... */
    }

    return page;
}

static void page_pool_free(void *pagebase, size_t npages)
{
    uintptr_t physpage = conv_kern_to_phys(pagebase);
    struct list *master = &page_master_table;
    struct list_elem *el;
    for (el = list_begin(master); el != list_end(master); el = list_next(el)) {
        struct page_pool *pool = elem_value(el, struct page_pool, master_elem);
        if (pool->base <= physpage && physpage < pool->base + pool->length) {
            size_t bitn = (physpage - pool->base) / FAKIX_PAGE;
            if (bitmap_all(pool->page_map, bitn, npages, 1)) {
                bitmap_set(pool->page_map, bitn, npages, 0);
            }
            break;
        }
    }
}