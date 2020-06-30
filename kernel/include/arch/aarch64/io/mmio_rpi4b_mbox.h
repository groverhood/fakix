#ifndef KERNEL_ARCH_AARCH64_IO_MMIO_RPI4B_MBOX_H
#define KERNEL_ARCH_AARCH64_IO_MMIO_RPI4B_MBOX_H 1

#include <sys/core.h>

#define MMIO_RPI4B_ARM_LOCAL_BASE 0xff800000

#define MBOX_SET00 *(volatile uint32_t)(MMIO_RPI4B_ARM_LOCAL_BASE + 0x80)
#define MBOX_SET01 *(volatile uint32_t)(MMIO_RPI4B_ARM_LOCAL_BASE + 0x84)
#define MBOX_SET02 *(volatile uint32_t)(MMIO_RPI4B_ARM_LOCAL_BASE + 0x88)
#define MBOX_SET03 *(volatile uint32_t)(MMIO_RPI4B_ARM_LOCAL_BASE + 0x8c)
#define MBOX_SET04 *(volatile uint32_t)(MMIO_RPI4B_ARM_LOCAL_BASE + 0x90)
#define MBOX_SET05 *(volatile uint32_t)(MMIO_RPI4B_ARM_LOCAL_BASE + 0x94)
#define MBOX_SET06 *(volatile uint32_t)(MMIO_RPI4B_ARM_LOCAL_BASE + 0x98)
#define MBOX_SET07 *(volatile uint32_t)(MMIO_RPI4B_ARM_LOCAL_BASE + 0x9c)
#define MBOX_SET08 *(volatile uint32_t)(MMIO_RPI4B_ARM_LOCAL_BASE + 0xa0)
#define MBOX_SET09 *(volatile uint32_t)(MMIO_RPI4B_ARM_LOCAL_BASE + 0xa4)
#define MBOX_SET10 *(volatile uint32_t)(MMIO_RPI4B_ARM_LOCAL_BASE + 0xa8)
#define MBOX_SET11 *(volatile uint32_t)(MMIO_RPI4B_ARM_LOCAL_BASE + 0xac)
#define MBOX_SET12 *(volatile uint32_t)(MMIO_RPI4B_ARM_LOCAL_BASE + 0xb0)
#define MBOX_SET13 *(volatile uint32_t)(MMIO_RPI4B_ARM_LOCAL_BASE + 0xb4)
#define MBOX_SET14 *(volatile uint32_t)(MMIO_RPI4B_ARM_LOCAL_BASE + 0xb8)
#define MBOX_SET15 *(volatile uint32_t)(MMIO_RPI4B_ARM_LOCAL_BASE + 0xbc)

#define MBOX_CLR00 *(volatile uint32_t)(MMIO_RPI4B_ARM_LOCAL_BASE + 0xc0)
#define MBOX_CLR01 *(volatile uint32_t)(MMIO_RPI4B_ARM_LOCAL_BASE + 0xc4)
#define MBOX_CLR02 *(volatile uint32_t)(MMIO_RPI4B_ARM_LOCAL_BASE + 0xc8)
#define MBOX_CLR03 *(volatile uint32_t)(MMIO_RPI4B_ARM_LOCAL_BASE + 0xcc)
#define MBOX_CLR04 *(volatile uint32_t)(MMIO_RPI4B_ARM_LOCAL_BASE + 0xd0)
#define MBOX_CLR05 *(volatile uint32_t)(MMIO_RPI4B_ARM_LOCAL_BASE + 0xd4)
#define MBOX_CLR06 *(volatile uint32_t)(MMIO_RPI4B_ARM_LOCAL_BASE + 0xd8)
#define MBOX_CLR07 *(volatile uint32_t)(MMIO_RPI4B_ARM_LOCAL_BASE + 0xdc)
#define MBOX_CLR08 *(volatile uint32_t)(MMIO_RPI4B_ARM_LOCAL_BASE + 0xe0)
#define MBOX_CLR09 *(volatile uint32_t)(MMIO_RPI4B_ARM_LOCAL_BASE + 0xe4)
#define MBOX_CLR10 *(volatile uint32_t)(MMIO_RPI4B_ARM_LOCAL_BASE + 0xe8)
#define MBOX_CLR11 *(volatile uint32_t)(MMIO_RPI4B_ARM_LOCAL_BASE + 0xec)
#define MBOX_CLR12 *(volatile uint32_t)(MMIO_RPI4B_ARM_LOCAL_BASE + 0xf0)
#define MBOX_CLR13 *(volatile uint32_t)(MMIO_RPI4B_ARM_LOCAL_BASE + 0xf4)
#define MBOX_CLR14 *(volatile uint32_t)(MMIO_RPI4B_ARM_LOCAL_BASE + 0xf8)
#define MBOX_CLR15 *(volatile uint32_t)(MMIO_RPI4B_ARM_LOCAL_BASE + 0xfc)

#endif // KERNEL_ARCH_AARCH64_IO_MMIO_RPI4B_MBOX_H
