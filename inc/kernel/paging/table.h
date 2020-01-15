#ifndef FAKIX_PAGING_TABLE_H
#define FAKIX_PAGING_TABLE_H 1

#include <stddef.h>

/* Page table entry. Its size is equivalent to the word size of the machine. */
typedef size_t pte_t;

/**
 *  Sets the active page table to the provided pointer.
 * 
 *  RETURN VALUES
 *      Nothing. There is no way to consistently predict the validity of a page
 *      table, so FAKIX will just let itself GP fault.
 **/
void paging_table_set_to(pte_t *root);

#endif
