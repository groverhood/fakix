#ifndef FAKIX_ADDR_H
#define FAKIX_ADDR_H 1

#include <stdint.h>

typedef uintptr_t vaddr_t;
typedef uintptr_t paddr_t;

#define PRIuVADDR "llu"
#define PRIxVADDR "llx"
#define PRIuPADDR "llu"
#define PRIxPADDR "llx"

#endif