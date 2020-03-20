#ifndef KERNEL_INT_ARCH_FRAME
#define KERNEL_INT_ARCH_FRAME 1

#include <stdint.h>
#include <attributes.h>

struct packed int_arch_frame {
    uint64_t error;
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
};

#endif