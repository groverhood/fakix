#ifndef KERNEL_ARCH_AARCH64_IO_MMIO_RPI4B_GPU_H
#define KERNEL_ARCH_AARCH64_IO_MMIO_RPI4B_GPU_H 1

#include <stdint.h>
#include <mfence.h>
#include <attributes.h>
#include <io/mmio_rpi4b_peripherals.h>

#define MMIO_RPI4B_GPU_MBOX_BASE (MMIO_RPI4B_PERIPHERAL_BASE + 0xb800)

#ifndef ASM_FILE

enum mmio_rpi4b_gpu_cmd {
    GPU_CMD_GET_CLOCK_STATE,
    GPU_CMD_SET_CLOCK_STATE,
    GPU_CMD_GET_CLOCK_RATE,
    GPU_CMD_GET_CLOCK_MAX_RATE,
    GPU_CMD_SET_CLOCK_RATE,
};

struct mmio_rpi4b_gpu_msg_hdr {
    uint32_t msg_size;
    uint32_t rsvd;
    enum mmio_rpi4b_gpu_cmd cmd;
    uint32_t resp_size;
    uint32_t req_size;
    uint32_t argv[32]; /* Null-terminated word array */
} align(16);

#define GPU_MSG_HDR_SIZ offsetof(struct mmio_rpi4b_gpu_msg_hdr, argv)

struct mmio_rpi4b_gpu_mbox {
    uint32_t read;
    uint32_t reserved0[3];
    uint32_t peak;
    uint32_t sender;
    uint32_t status;
    uint32_t config;
    uint32_t write;
};

enum mmio_rpi4b_gpu_mbox_channel {
    GPU_CHANNEL_POWER,
    GPU_CHANNEL_FRAMEBUF,
    GPU_CHANNEL_VUART,
    GPU_CHANNEL_VCHIQ,
    GPU_CHANNEL_LEDS,
    GPU_CHANNEL_BUTTONS,
    GPU_CHANNEL_TOUCHSCREEN,
    GPU_CHANNEL_COUNTER,
    GPU_CHANNEL_TAGS,
    GPU_CHANNEL_GPU,

    GPU_CHANNEL_COUNT
};

static inline uint32_t arm_to_vc_addr(void *addr)
{
    return (uint32_t)addr | 0xc0000000;
}

static void mmio_rpi4b_gpu_mbox_write(enum mmio_rpi4b_gpu_mbox_channel channel, struct mmio_rpi4b_gpu_msg_hdr *msg)
{
    volatile struct mmio_rpi4b_gpu_mbox *mb = (volatile struct mmio_rpi4b_gpu_mbox *)(MMIO_RPI4B_GPU_MBOX_BASE);
    if (channel < GPU_CHANNEL_COUNT) {
        uint32_t data = arm_to_vc_addr(msg);
        while (mb->status & (1 << 31));
        mfence();
        mb->write = ((data & ~0xf) | (channel));
    }
}

static uint32_t mmio_rpi4b_gpu_mbox_read(enum mmio_rpi4b_gpu_mbox_channel channel)
{
    volatile struct mmio_rpi4b_gpu_mbox *mb = (volatile struct mmio_rpi4b_gpu_mbox *)(MMIO_RPI4B_GPU_MBOX_BASE);
    uint32_t res = (uint32_t)-1;
    if (channel < GPU_CHANNEL_COUNT) {
        uint32_t data;
        do {
            while (mb->status & (1 << 30));
            mfence();
            data = mb->read;
        } while ((data & 0xf) != channel);
        res = data;
    }
    return res;
}

enum mmio_rpi4b_gpu_clock {
    GPU_CLOCK_EMMC = 1,
    GPU_CLOCK_UART,
    GPU_CLOCK_ARM,
    GPU_CLOCK_CORE,
    GPU_CLOCK_V3D,
    GPU_CLOCK_H264,
    GPU_CLOCK_ISP,
    GPU_CLOCK_SDRAM,
    GPU_CLOCK_PIXEL,
    GPU_CLOCK_PWM
};

#endif

#endif // KERNEL_ARCH_AARCH64_IO_MMIO_RPI4B_GPU_H
