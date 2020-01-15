/**
 *  === table.c ===
 * 
 *  x86_64-specific implementation of paging.
 * 
 *  Entry points:
 *      - void paging_table_set_to(pte_t *root);
 **/

#include <paging/table.h>

void paging_table_set_to(pte_t *root)
{
    __asm__ __volatile__ (
        "movq %[ROOT], %%cr3"
        :: [ROOT] "D" (root)
    );
}