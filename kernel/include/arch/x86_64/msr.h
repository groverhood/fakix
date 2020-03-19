#ifndef KERNEL_ARCH_X86_64_MSR_H
#define KERNEL_ARCH_X86_64_MSR_H 1

#define IA32_EFER_MSR 0xC0000080

#ifndef ASM_FILE

#include <stdint.h>

typedef uintptr_t msraddr_t;

static inline void wrmsr(msraddr_t msr, uint64_t value)
{
    asm volatile (
        "wrmsr\n\t"
        :: "c" (msr), "a" (value & UINT32_MAX), "d" (value >> 32)
    );
}

static inline uint64_t rdmsr(msraddr_t msr)
{
    uint64_t value;
    uint32_t valuel, valueh;
    asm volatile (
        "rdmsr"
        : "=a" (valuel), "=d" (valueh)
        : "c" (msr)
    );

    value = ((uint64_t)valueh << 32) | ((uint64_t)valuel);
    return value;
}

#endif

#endif