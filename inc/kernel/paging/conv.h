#ifndef FAKIX_PAGING_CONV_H
#define FAKIX_PAGING_CONV_H

#include <stdint.h>

#define FAKIX_PHYSBASE ((uintptr_t)0xFFFF800000000000)

static inline void *conv_phys_to_kern(uintptr_t phys)
{
    return (void *)(phys + FAKIX_PHYSBASE);
}

static inline uintptr_t conv_kern_to_phys(void *kern)
{
    return ((uintptr_t)kern - FAKIX_PHYSBASE);
}

#endif