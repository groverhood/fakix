#ifndef ARCH_X86_64_FAKIX_SYSCALL_ARCH_H
#define ARCH_X86_64_FAKIX_SYSCALL_ARCH_H 1

#include <stdint.h>
#include <fakix/errtype.h>

typedef uint64_t sysarg_t;

static inline errval_t syscall_arch(sysarg_t arg0, sysarg_t arg1, sysarg_t arg2, sysarg_t arg3,
                      sysarg_t arg4, sysarg_t arg5, sysarg_t sysn)
{
    register errval_t err asm("rax");

    asm volatile (
        "movq %%rcx, %%r10\n\t"
        "syscall"
        : "=a" (err)
        : "a" (sysn)
        : "r10", "r11", "rcx"
    );

    return err;
}

#endif