#ifndef ARCH_X86_64_CPU_FSGS_H
#define ARCH_X86_64_CPU_FSGS_H 1

#include <stdint.h>

static inline void write_fs_base(uint64_t fsbase)
{
    asm volatile ("wrfsbase %%rdi" :: "D" (fsbase));
}

static inline void write_gs_base(uint64_t gsbase)
{
    asm volatile ("wrgsbase %%rdi" :: "D" (gsbase));
}

static inline uint64_t read_fs_base(void)
{
    uint64_t fsbase;
    asm volatile ("rdfsbase %%rax" : "=a" (fsbase));
    return fsbase;
}

static inline uint64_t read_gs_base(void)
{
    uint64_t gsbase;
    asm volatile ("rdgsbase %%rax" : "=a" (gsbase));
    return gsbase;
}

#endif