#ifndef FAKIX_PAGING_ALLOC_H
#define FAKIX_PAGING_ALLOC_H 1

#include <fakix.h>
#include <stddef.h>

void init_page_alloc(struct fakix_memmap *mmap);

typedef int pal_flags_t;

#define PAGE_ALLOC_ZERO   0b00000001
#define PAGE_ALLOC_ASSERT 0b00000010
#define PAGE_ALLOC_SIZE4K 0b00000100
#define PAGE_ALLOC_SIZE2M 0b00001000
#define PAGE_ALLOC_SIZE1G 0b00001100
#define PAGE_ALLOC_USER   0b00010000

#define PAGE_ALLOC_SIZEMASK PAGE_ALLOC_SIZE1G

void *page_allocate_multiple(pal_flags_t al_flags, size_t npages);
void *page_allocate(pal_flags_t al_flags);

void page_free_multiple(void *pagebase, int size, size_t npages);
void page_free(void *page, int size);

#endif