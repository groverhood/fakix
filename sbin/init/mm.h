#ifndef SBIN_INIT_MM_H
#define SBIN_INIT_MM_H 1

#include <stddef.h>
#include <fakix/cspace.h>

void mm_init(void);
void mm_alloc_aligned(size_t size, size_t alignment, capaddr_t *ret_cap);

#endif