#ifndef KERNEL_ARCH_X86_64_INIT_BOOTINFO_ARCH_H
#define KERNEL_ARCH_X86_64_INIT_BOOTINFO_ARCH_H 1

#include <fakix/cspace.h>

struct bootinfo_arch {
    capaddr_t idt_cap;
};

#endif