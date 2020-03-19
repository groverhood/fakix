#ifndef KERNEL_INT_FRAME_H
#define KERNEL_INT_FRAME_H 1

#include <int/idt_ent.h>
#include <cpu/registers.h>
#include <int/arch_frame.h>

typedef uint64_t ierror_t;

struct int_frame {
    ivec_t inum;
    ierror_t errcode;
    union registers context;
    struct int_arch_frame arch;
};

#endif