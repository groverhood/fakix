#ifndef KERNEL_ARCH_X86_64_INT_ISR_H
#define KERNEL_ARCH_X86_64_INT_ISR_H 1

#include <attributes.h>
#include <int/redirect.h>
#include <fakix/tcb_arch.h>

void isr_common_stub(ivec_t ivec);

#endif