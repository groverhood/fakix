#ifndef KERNEL_ARCH_X86_64_MSR_H
#define KERNEL_ARCH_X86_64_MSR_H 1

#define IA32_APIC_MSR 0
#define IA32_EFER_MSR 0xc0000080
#define IA32_LSTAR_MSR 0xc0000082

#ifndef ASM_FILE

#include <stdint.h>
#include <fakix/errors.h>
#include <fakix/errtype.h>

typedef uintptr_t msraddr_t;

static inline errval_t wrmsr(msraddr_t msr, uint64_t value)
{
    if (msr > UINT32_MAX) {
        return KERNEL_ERR_WRMSR_INVALID_MSR;
    }

    asm volatile (
        "wrmsr\n\t"
        :: "c" (msr), "a" (value & UINT32_MAX), "d" (value >> 32)
    );

    return ERR_OK;
}

static inline errval_t rdmsr(msraddr_t msr, uint64_t *ret_value)
{
    uint32_t valuel, valueh;
    if (msr > UINT32_MAX) {
        return KERNEL_ERR_WRMSR_INVALID_MSR;
    }

    asm volatile (
        "rdmsr"
        : "=a" (valuel), "=d" (valueh)
        : "c" (msr)
    );

    *ret_value = ((uint64_t)valueh << 32) | ((uint64_t)valuel);
    return ERR_OK;
}

#endif

#endif