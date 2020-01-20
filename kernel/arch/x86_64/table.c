/**
 *  === table.c ===
 * 
 *  x86_64-specific implementation of paging.
 * 
 *  Entry points:
 *      - void paging_table_set_to(pte_t *root);
 **/

#include <paging/conv.h>
#include <paging/table.h>

#define P4_INDEX_MASK ((size_t)0x1FF << 39)
#define P3_INDEX_MASK ((size_t)0x1FF << 30)
#define P2_INDEX_MASK ((size_t)0x1FF << 21)
#define P1_INDEX_MASK ((size_t)0x1FF << 12)

#define page_level_index(addr, index) ((addr & P##index##_INDEX_MASK) >> (12 + 9 * (index - 1)))

void paging_table_init(pte_t *kernel_root)
{
    paging_table_set_to(kernel_root);
}

void paging_table_set_to(pte_t *root)
{
    __asm__ __volatile__ (
        "movq %[ROOT], %%cr3"
        :: [ROOT] "D" (root)
    );
}

void *allocate_page_table(void)
{
}

void paging_table_map(pte_t *root, void *virtual_addr, uintptr_t phys_addr)
{
    uintptr_t logical = (uintptr_t)virtual_addr;

    size_t p4_index = page_level_index(logical, 4);
    pte_t *p3_table = (pte_t *)(root[p4_index] & ~0xFFF);


}

void paging_table_unmap(pte_t *root, void *virtual_addr)
{

}