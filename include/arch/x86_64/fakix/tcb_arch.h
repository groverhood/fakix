#ifndef ARCH_X86_64_FAKIX_TCB_ARCH_H
#define ARCH_X86_64_FAKIX_TCB_ARCH_H 1

#include <stddef.h>
#include <stdint.h>
#include <cpu/registers.h>
#include <cpu/fpu_state.h>

struct tcb_arch_shared {
    uint16_t cs, ds;
    uint16_t fs, gs;
    regval_t flags;

    size_t *pml4;
};

struct tcb_arch {
    struct tcb_arch_shared shared;
};

#endif