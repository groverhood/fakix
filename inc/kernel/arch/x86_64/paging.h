#ifndef FAKIX_ARCH_X86_64_PAGING_H
#define FAKIX_ARCH_X86_64_PAGING_H 1

#include <arch/x86_64/load.h>

#define FAKIX_KERN_PAGE_TABLE ((pte_t *)FAKIX_KERN_PML4)

#endif