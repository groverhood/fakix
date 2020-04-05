#ifndef ARCH_X86_64_FAKIX_TCB_CURRENT_H
#define ARCH_X86_64_FAKIX_TCB_CURRENT_H 1

#include <fakix/tcb.h>

tcb_handle_t tcb_current(void)
{
    tcb_handle_t tcb;
    asm volatile ("movq %%fs:8, %0" : "=r" (tcb));
    return tcb;
}

#endif