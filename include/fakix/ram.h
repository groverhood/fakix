#ifndef FAKIX_RAM_H
#define FAKIX_RAM_H 1

#include <stddef.h>
#include <fakix/capabilities.h>

/* Function to allocate caps with. */
typedef errval_t ram_alloc_func_t(size_t size, size_t alignment, capaddr_t *ret_cap);

errval_t ram_alloc_aligned(size_t size, size_t alignment, capaddr_t *ret_cap);
errval_t ram_alloc(size_t size, capaddr_t *ret_cap);
errval_t ram_free(capaddr_t ram_cap);

errval_t ram_set_alloc_func(ram_alloc_func_t *alloc_functor);
ram_alloc_func_t *ram_get_default_alloc_func(void);
void ram_alloc_init(void);

errval_t ram_cap_info(capaddr_t ram_cap, paddr_t *ret_base, size_t *ret_bytes);

#endif