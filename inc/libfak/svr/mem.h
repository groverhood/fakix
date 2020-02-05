#ifndef FAKIX_LIBFAK_SVR_MEM_H
#define FAKIX_LIBFAK_SVR_MEM_H 1

#include <cbl.h>
#include <stdint.h>

typedef int memerr_t;

memerr_t mem_map_page(void *vaddr, uintptr_t paddr, struct capability cap);

#endif