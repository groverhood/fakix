#ifndef KERNEL_ARCH_AARCH64_IO_MMIO_RPI4B_PERIPHERALS_H
#define KERNEL_ARCH_AARCH64_IO_MMIO_RPI4B_PERIPHERALS_H 1

#define MMIO_RPI4B_PERIPHERAL_BASE 0xfe000000ul

#define MMIO_RPI4B_UART0_BASE (MMIO_RPI4B_PERIPHERAL_BASE + 0x201000)
#define MMIO_RPI4B_UART2_BASE (MMIO_RPI4B_PERIPHERAL_BASE + 0x201400)
#define MMIO_RPI4B_UART3_BASE (MMIO_RPI4B_PERIPHERAL_BASE + 0x201600)
#define MMIO_RPI4B_UART4_BASE (MMIO_RPI4B_PERIPHERAL_BASE + 0x201800)
#define MMIO_RPI4B_UART5_BASE (MMIO_RPI4B_PERIPHERAL_BASE + 0x201a00)

#endif
