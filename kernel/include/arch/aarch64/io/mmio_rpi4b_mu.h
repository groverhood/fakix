#ifndef KERNEL_ARCH_AARCH64_IO_MMIO_RPI4B_MU_H
#define KERNEL_ARCH_AARCH64_IO_MMIO_RPI4B_MU_H 1

#include <mfence.h>
#include <io/mmio_rpi4b_peripherals.h>

/* Auxiliary registers (UART1M SPI1 & SPI2). */

#define MMIO_RPI4B_AUX_BASE (MMIO_RPI4B_PERIPHERAL_BASE + 0x215000)
#define MMIO_RPI4B_AUX_IRQ 0x00
#define MMIO_RPI4B_AUX_ENABLES 0x04
#define MMIO_RPI4B_AUX_MU_IO_REG 0x40
#define MMIO_RPI4B_AUX_MU_IER_REG 0x44
#define MMIO_RPI4B_AUX_MU_IIR_REG 0x48
#define MMIO_RPI4B_AUX_MU_LCR_REG 0x4c
#define MMIO_RPI4B_AUX_MU_MCR_REG 0x50
#define MMIO_RPI4B_AUX_MU_LSR_REG 0x54
#define MMIO_RPI4B_AUX_MU_MSR_REG 0x58
#define MMIO_RPI4B_AUX_MU_SCRATCH 0x5c
#define MMIO_RPI4B_AUX_MU_CNTL_REG 0x60
#define MMIO_RPI4B_AUX_MU_STAT_REG 0x64
#define MMIO_RPI4B_AUX_MU_BAUD_REG 0x68

#define MMIO_RPI4B_AUX_SPI1_CNTL0_REG 0x80
#define MMIO_RPI4B_AUX_SPI1_CNTL1_REG 0x84
#define MMIO_RPI4B_AUX_SPI1_STAT_REG 0x88
#define MMIO_RPI4B_AUX_SPI1_PEEK_REG 0x8c

/* 16-byte aligned data registers. */
#define MMIO_RPI4B_AUX_SPI1_IO_REG 0xa0
/* 16-byte aligned extended data register. */
#define MMIO_RPI4B_AUX_SPI1_TXHOLD_REG 0xb0

#define MMIO_RPI4B_AUX_SPI2_CNTL0_REG 0xc0
#define MMIO_RPI4B_AUX_SPI2_CNTL1_REG 0xc4
#define MMIO_RPI4B_AUX_SPI2_STAT_REG 0xc8
#define MMIO_RPI4B_AUX_SPI2_PEEK_REG 0xcc

/* 16-byte aligned data register. */
#define MMIO_RPI4B_AUX_SPI2_IO_REG 0xe0
/* 16-byte aligned extended data register. */
#define MMIO_RPI4B_AUX_SPI2_TXHOLD_REG 0xf0


#ifndef ASM_FILE

/* Auxiliary utility functions. These are simply functional abstractions for 
   ugly reading/writing operations. The programmer will need to perform
   extraneous register checks through the additional function wrappers provided. */

#include <stddef.h>
#include <stdbool.h>
#include <fakix/addr.h>
#include <attributes.h>

enum mmio_rpi4b_aux_dev {
    MMIO_RPI4B_AUX_DEV_MINI_UART,
    MMIO_RPI4B_AUX_DEV_SPI1,
    MMIO_RPI4B_AUX_DEV_SPI2
};

static inline bool mmio_rpi4b_aux_irq_pending(paddr_t aux_base, enum mmio_rpi4b_aux_dev dev)
{
    mfence();
    return (*((volatile uint8_t *)aux_base + MMIO_RPI4B_AUX_IRQ) & (1 << dev)); /* Coerce to bool. */
}

static inline void mmio_rpi4b_aux_enable(paddr_t aux_base, enum mmio_rpi4b_aux_dev dev)
{
    mfence();
    *((volatile uint8_t *)aux_base + MMIO_RPI4B_AUX_ENABLES) |= (1 << dev);
}

static inline void mmio_rpi4b_aux_disable(paddr_t aux_base, enum mmio_rpi4b_aux_dev dev)
{
    mfence();
    *((volatile uint8_t *)aux_base + MMIO_RPI4B_AUX_ENABLES) &= ~(1 << dev);
}

static inline uint8_t mmio_rpi4b_aux_mu_io_reg_rd(paddr_t aux_base)
{
    mfence();
    return *((volatile uint8_t *)aux_base + MMIO_RPI4B_AUX_MU_IO_REG);
}

static inline void mmio_rpi4b_aux_mu_io_reg_wr(paddr_t aux_base, uint8_t b)
{
    mfence();
    *((volatile uint8_t *)aux_base + MMIO_RPI4B_AUX_MU_IO_REG) = b;
}

static inline void mmio_rpi4b_aux_mu_ier_reg_set(paddr_t aux_base)
{
    mfence();
    *((volatile uint8_t *)aux_base + MMIO_RPI4B_AUX_MU_IER_REG) = 1;
}

static inline void mmio_rpi4b_aux_mu_ier_reg_clear(paddr_t aux_base)
{
    mfence();
    *((volatile uint8_t *)aux_base + MMIO_RPI4B_AUX_MU_IER_REG) = 0;
}

/* Declare as volatile. */
struct mmio_rpi4b_aux_mu_iir_reg {
    uint8_t interrupt_pending : 1;
    uint8_t interrupt_id_fifo_clear : 2; /* On read shows the interrupt ID bit, 
                                            on write clears the receive/transmit
                                            FIFO. */
    uint8_t zero : 3;
    uint8_t fifo_enables : 2;
    uint8_t pad[3];
} packed;

#define MMIO_RPI4_AUX_MU_IIR_REG volatile struct mmio_rpi4b_aux_mu_iir_reg *
#define MMIO_WORD_ASSERT(type) \
    _Static_assert(sizeof(type) == sizeof(uint32_t), "Improper size [" #type "]")

MMIO_WORD_ASSERT(struct mmio_rpi4b_aux_mu_iir_reg);

static inline bool mmio_rpi4b_aux_iir_reg_interrupt_pending(paddr_t aux_base)
{
    mfence();
    MMIO_RPI4_AUX_MU_IIR_REG reg = (MMIO_RPI4_AUX_MU_IIR_REG)(aux_base + MMIO_RPI4B_AUX_MU_IIR_REG);
    return reg->interrupt_pending;
}

static inline int mmio_rpi4b_aux_iir_reg_interrupt_id_bits(paddr_t aux_base)
{
    mfence();
    MMIO_RPI4_AUX_MU_IIR_REG reg = (MMIO_RPI4_AUX_MU_IIR_REG)(aux_base + MMIO_RPI4B_AUX_MU_IIR_REG);
    return reg->interrupt_id_fifo_clear;
}

enum mmio_rpi4b_fifo {
    MMIO_RPI4_FIFO_RECEIVE,
    MMIO_RPI4_FIFO_TRANSMIT
};

static inline void mmio_rpi4b_aux_iir_reg_clear_fifo_bits(paddr_t aux_base, enum mmio_rpi4b_fifo id)
{
    mfence();
    MMIO_RPI4_AUX_MU_IIR_REG reg = (MMIO_RPI4_AUX_MU_IIR_REG)(aux_base + MMIO_RPI4B_AUX_MU_IIR_REG);
    reg->interrupt_id_fifo_clear &= ~(1 << id);
}

enum mmio_rpi4b_interrupt_id {
    MMIO_RPI4_INTERRUPT_ID_NO_INTERRUPTS,
    MMIO_RPI4_INTERRUPT_ID_TRANSMIT_EMPTY,
    MMIO_RPI4_INTERRUPT_ID_RECEIVER_VALID,
    MMIO_RPI4_INTERRUPT_ID_IMPOSSIBLE
};

static inline enum mmio_rpi4b_interrupt_id mmio_rpi4b_aux_iir_read_interrupt_id_bits(paddr_t aux_base)
{
    mfence();
    MMIO_RPI4_AUX_MU_IIR_REG reg = (MMIO_RPI4_AUX_MU_IIR_REG)(aux_base + MMIO_RPI4B_AUX_MU_IIR_REG);
    return reg->interrupt_id_fifo_clear;
}

struct mmio_rpi4b_aux_mu_lcr_reg {
    uint8_t data_size : 1;
    uint8_t reserved : 5;
    uint8_t brk : 1;
    uint8_t zero : 1;
    uint8_t pad[3];
} packed;

#define MMIO_RPI4_AUX_MU_LCR_REG volatile struct mmio_rpi4b_aux_mu_lcr_reg *
MMIO_WORD_ASSERT(struct mmio_rpi4b_aux_mu_lcr_reg);

enum mmio_rpi4b_aux_mu_lcr_datasize {
    MMIO_RPI4_AUX_MU_LCR_DATASIZE_7BIT,
    MMIO_RPI4_AUX_MU_LCR_DATASIZE_8BIT
};

static inline void mmio_rpi4b_aux_mu_lcr_datasize_wr(paddr_t aux_base, enum mmio_rpi4b_aux_mu_lcr_datasize datasize)
{
    mfence();
    MMIO_RPI4_AUX_MU_LCR_REG reg = (MMIO_RPI4_AUX_MU_LCR_REG)(aux_base + MMIO_RPI4B_AUX_MU_IIR_REG);
    reg->data_size = datasize;
}

static inline enum mmio_rpi4b_aux_mu_lcr_datasize mmio_rpi4b_aux_mu_lcr_datasize_rd(paddr_t aux_base, enum mmio_rpi4b_aux_mu_lcr_datasize datasize)
{
    mfence();
    MMIO_RPI4_AUX_MU_LCR_REG reg = (MMIO_RPI4_AUX_MU_LCR_REG)(aux_base + MMIO_RPI4B_AUX_MU_IIR_REG);
    return reg->data_size;
}

static inline void mmio_rpi4b_aux_mu_lcr_brk_wr(paddr_t aux_base, bool brk)
{
    mfence();
    MMIO_RPI4_AUX_MU_LCR_REG reg = (MMIO_RPI4_AUX_MU_LCR_REG)(aux_base + MMIO_RPI4B_AUX_MU_IIR_REG);
    reg->brk = brk;
}

static inline bool mmio_rpi4b_aux_mu_lcr_brk_rd(paddr_t aux_base, bool brk)
{
    mfence();
    MMIO_RPI4_AUX_MU_LCR_REG reg = (MMIO_RPI4_AUX_MU_LCR_REG)(aux_base + MMIO_RPI4B_AUX_MU_IIR_REG);
    return reg->brk;
}

static inline bool mmio_rpi4b_aux_mu_mcr_reg_uart1_rts_low(paddr_t aux_base)
{
    mfence();
    return (*((volatile uint8_t *)aux_base + MMIO_RPI4B_AUX_MU_MCR_REG) & (1 << 1));
}

static inline bool mmio_rpi4b_aux_mu_mcr_reg_uart1_rts_high(paddr_t aux_base)
{
    mfence();
    return !(*((volatile uint8_t *)aux_base + MMIO_RPI4B_AUX_MU_MCR_REG) & (1 << 1));
}

static inline void mmio_rpi4b_aux_mu_mcr_reg_uart1_rts_wr(paddr_t aux_base, bool hl)
{
    mfence();
    *((volatile uint8_t *)aux_base + MMIO_RPI4B_AUX_MU_MCR_REG) = ((int)hl << 1);
}

struct mmio_rpi4b_aux_mu_lsr_reg {
    uint8_t data_ready : 1;
    uint8_t receiver_overrun : 1;
    uint8_t reserved : 3;
    uint8_t transmitter_empty : 1;
    uint8_t transmitter_idle : 1;
    uint8_t zero : 1;
    uint8_t pad[3];
} packed;

#define MMIO_RPI4_AUX_MU_LSR_REG volatile struct mmio_rpi4b_aux_mu_lsr_reg *
MMIO_WORD_ASSERT(struct mmio_rpi4b_aux_mu_lsr_reg);

static inline bool mmio_rpi4b_aux_mu_lsr_reg_data_ready(paddr_t aux_base)
{
    mfence();
    MMIO_RPI4_AUX_MU_LSR_REG reg = (MMIO_RPI4_AUX_MU_LSR_REG)(aux_base + MMIO_RPI4B_AUX_MU_LSR_REG);
    return reg->data_ready;
}

static inline bool mmio_rpi4b_aux_mu_lsr_reg_recv_overrun_clear(paddr_t aux_base)
{
    mfence();
    MMIO_RPI4_AUX_MU_LSR_REG reg = (MMIO_RPI4_AUX_MU_LSR_REG)(aux_base + MMIO_RPI4B_AUX_MU_LSR_REG);
    return reg->receiver_overrun;
}

static inline bool mmio_rpi4b_aux_mu_lsr_reg_transmitter_empty(paddr_t aux_base)
{
    mfence();
    MMIO_RPI4_AUX_MU_LSR_REG reg = (MMIO_RPI4_AUX_MU_LSR_REG)(aux_base + MMIO_RPI4B_AUX_MU_LSR_REG);
    return reg->transmitter_empty;
}

static inline bool mmio_rpi4b_aux_mu_lsr_reg_transmitter_idle(paddr_t aux_base)
{
    mfence();
    MMIO_RPI4_AUX_MU_LSR_REG reg = (MMIO_RPI4_AUX_MU_LSR_REG)(aux_base + MMIO_RPI4B_AUX_MU_LSR_REG);
    return reg->transmitter_idle;
}

static inline bool mmio_rpi4b_aux_mu_msr_reg_cts_get_status(paddr_t aux_base)
{
    mfence();
    /* Bit 4 == CTS status bit. 
       This bit is the inverse of the UART1_CTS input. Thus:
       If set the UART1_CTS pin is low;
       If clear the UART1_CTS pin is high. */
    return (*((volatile uint8_t *)aux_base + MMIO_RPI4B_AUX_MU_MSR_REG) & (1 << 4));
}

static inline void mmio_rpi4b_aux_mu_msr_reg_cts_set_status(paddr_t aux_base, bool status)
{
    mfence();
    *((volatile uint8_t *)aux_base + MMIO_RPI4B_AUX_MU_MSR_REG) = (int)status << 4;
}

static inline uint8_t mmio_rpi4b_aux_mu_scratch_read(paddr_t aux_base)
{
    mfence();
    return *((volatile uint8_t *)aux_base + MMIO_RPI4B_AUX_MU_MSR_REG);
}

static inline uint8_t mmio_rpi4b_aux_mu_scratch_write(paddr_t aux_base, uint8_t b)
{
    mfence();
    volatile uint8_t *scratch = ((uint8_t *)aux_base + MMIO_RPI4B_AUX_MU_MSR_REG);
    uint8_t old = *scratch;
    *scratch = b;
    return old;
}

struct mmio_rpi4b_aux_mu_cntl_reg {
    uint8_t receiver_enable : 1;
    uint8_t transmitter_enable : 1;
    uint8_t rts_deassert_autoflow : 1;
    uint8_t transmitter_stop_cts_deassert : 1;
    uint8_t rts_auto_flow : 2;
    uint8_t rts_assert_level : 1;
    uint8_t cts_assert_level : 1;
    uint8_t pad[3];
} packed;

#define MMIO_RPI4_AUX_MU_CNTL_REG volatile struct mmio_rpi4b_aux_mu_cntl_reg *
MMIO_WORD_ASSERT(struct mmio_rpi4b_aux_mu_cntl_reg);

static inline void mmio_rpi4b_aux_mu_cntl_reg_receiver_enable(paddr_t aux_base)
{
    mfence();
    MMIO_RPI4_AUX_MU_CNTL_REG reg = (MMIO_RPI4_AUX_MU_CNTL_REG)(aux_base + MMIO_RPI4B_AUX_MU_CNTL_REG);
    reg->receiver_enable = true;
}

static inline void mmio_rpi4b_aux_mu_cntl_reg_receiver_disable(paddr_t aux_base)
{
    mfence();
    MMIO_RPI4_AUX_MU_CNTL_REG reg = (MMIO_RPI4_AUX_MU_CNTL_REG)(aux_base + MMIO_RPI4B_AUX_MU_CNTL_REG);
    reg->receiver_enable = false;
}

static inline void mmio_rpi4b_aux_mu_cntl_reg_transmitter_enable(paddr_t aux_base)
{
    mfence();
    MMIO_RPI4_AUX_MU_CNTL_REG reg = (MMIO_RPI4_AUX_MU_CNTL_REG)(aux_base + MMIO_RPI4B_AUX_MU_CNTL_REG);
    reg->transmitter_enable = true;
}

static inline void mmio_rpi4b_aux_mu_cntl_reg_transmitter_disable(paddr_t aux_base)
{
    mfence();
    MMIO_RPI4_AUX_MU_CNTL_REG reg = (MMIO_RPI4_AUX_MU_CNTL_REG)(aux_base + MMIO_RPI4B_AUX_MU_CNTL_REG);
    reg->transmitter_enable = false;
}

static inline void mmio_rpi4b_aux_mu_cntl_reg_stop_transmitter_on_cts_deassert(paddr_t aux_base)
{
    mfence();
    MMIO_RPI4_AUX_MU_CNTL_REG reg = (MMIO_RPI4_AUX_MU_CNTL_REG)(aux_base + MMIO_RPI4B_AUX_MU_CNTL_REG);
    reg->rts_deassert_autoflow = true;
}

static inline void mmio_rpi4b_aux_mu_cntl_reg_ignore_cts_deassert(paddr_t aux_base)
{
    mfence();
    MMIO_RPI4_AUX_MU_CNTL_REG reg = (MMIO_RPI4_AUX_MU_CNTL_REG)(aux_base + MMIO_RPI4B_AUX_MU_CNTL_REG);
    reg->rts_deassert_autoflow = false;
}

enum mmio_rpi4b_rts_auto_flow_level {
    MMIO_RPI4B_RTS_AUTO_FLOW_DISABLE3,
    MMIO_RPI4B_RTS_AUTO_FLOW_DISABLE2,
    MMIO_RPI4B_RTS_AUTO_FLOW_DISABLE1,
    MMIO_RPI4B_RTS_AUTO_FLOW_DISABLE4,
};

static inline enum mmio_rpi4b_rts_auto_flow_level mmio_rpi4b_aux_mu_cntl_reg_get_rts_auto_flow_level(paddr_t aux_base)
{
    mfence();
    MMIO_RPI4_AUX_MU_CNTL_REG reg = (MMIO_RPI4_AUX_MU_CNTL_REG)(aux_base + MMIO_RPI4B_AUX_MU_CNTL_REG);
    return reg->rts_auto_flow;
}

static inline void mmio_rpi4b_aux_mu_cntl_reg_set_rts_auto_flow_level(paddr_t aux_base, enum mmio_rpi4b_rts_auto_flow_level level)
{
    mfence();
    MMIO_RPI4_AUX_MU_CNTL_REG reg = (MMIO_RPI4_AUX_MU_CNTL_REG)(aux_base + MMIO_RPI4B_AUX_MU_CNTL_REG);
    reg->rts_auto_flow = level;
}

static inline void mmio_rpi4b_aux_mu_cntl_reg_rts_auto_flow_assert_level_set_low(paddr_t aux_base)
{
    mfence();
    MMIO_RPI4_AUX_MU_CNTL_REG reg = (MMIO_RPI4_AUX_MU_CNTL_REG)(aux_base + MMIO_RPI4B_AUX_MU_CNTL_REG);
    reg->rts_assert_level = 1;
}

static inline void mmio_rpi4b_aux_mu_cntl_reg_rts_auto_flow_assert_level_set_high(paddr_t aux_base)
{
    mfence();
    MMIO_RPI4_AUX_MU_CNTL_REG reg = (MMIO_RPI4_AUX_MU_CNTL_REG)(aux_base + MMIO_RPI4B_AUX_MU_CNTL_REG);
    reg->rts_assert_level = 0;
}

static inline void mmio_rpi4b_aux_mu_cntl_reg_cts_auto_flow_assert_level_set_low(paddr_t aux_base)
{
    mfence();
    MMIO_RPI4_AUX_MU_CNTL_REG reg = (MMIO_RPI4_AUX_MU_CNTL_REG)(aux_base + MMIO_RPI4B_AUX_MU_CNTL_REG);
    reg->cts_assert_level = 1;
}

static inline void mmio_rpi4b_aux_mu_cntl_reg_cts_auto_flow_assert_level_set_high(paddr_t aux_base)
{
    mfence();
    MMIO_RPI4_AUX_MU_CNTL_REG reg = (MMIO_RPI4_AUX_MU_CNTL_REG)(aux_base + MMIO_RPI4B_AUX_MU_CNTL_REG);
    reg->cts_assert_level = 0;
}

struct mmio_rpi4b_aux_mu_stat_reg {
    uint8_t symbol_available : 1; /* Receiver has a symbol ready to be read. */
    uint8_t space_available : 1; /* Transmitter can accept at least more than one symbol. */
    uint8_t receiver_idle : 1;
    uint8_t transmitter_idle : 1;
    uint8_t receiver_overrun : 1;
    uint8_t transmitter_full : 1;
    uint8_t rts_status : 1;
    uint8_t cts_status : 1;
    uint8_t transmitter_empty : 1;
    uint8_t transmitter_done : 1;
    uint8_t rsvd : 6;
    uint8_t receiver_symbol_count : 4;
    uint8_t zero : 4;
    uint8_t transmitter_symbol_count : 4;
    uint8_t pad : 4;
} packed;

#define MMIO_RPI4_AUX_MU_STAT_REG volatile struct mmio_rpi4b_aux_mu_stat_reg *
MMIO_WORD_ASSERT(struct mmio_rpi4b_aux_mu_stat_reg);

static inline bool mmio_rpi4b_aux_mu_stat_reg_symbol_available(paddr_t aux_base)
{
    mfence();
    MMIO_RPI4_AUX_MU_STAT_REG reg = (MMIO_RPI4_AUX_MU_STAT_REG)(aux_base + MMIO_RPI4B_AUX_MU_STAT_REG);
    return reg->symbol_available;
}

static inline bool mmio_rpi4b_aux_mu_stat_reg_space_available(paddr_t aux_base)
{
    mfence();
    MMIO_RPI4_AUX_MU_STAT_REG reg = (MMIO_RPI4_AUX_MU_STAT_REG)(aux_base + MMIO_RPI4B_AUX_MU_STAT_REG);
    return reg->space_available;
}

static inline bool mmio_rpi4b_aux_mu_stat_reg_receiver_idle(paddr_t aux_base)
{
    mfence();
    MMIO_RPI4_AUX_MU_STAT_REG reg = (MMIO_RPI4_AUX_MU_STAT_REG)(aux_base + MMIO_RPI4B_AUX_MU_STAT_REG);
    return reg->receiver_idle;
}

static inline bool mmio_rpi4b_aux_mu_stat_reg_transmitter_idle(paddr_t aux_base)
{
    mfence();
    MMIO_RPI4_AUX_MU_STAT_REG reg = (MMIO_RPI4_AUX_MU_STAT_REG)(aux_base + MMIO_RPI4B_AUX_MU_STAT_REG);
    return reg->transmitter_idle;
}

static inline bool mmio_rpi4b_aux_mu_stat_reg_receiver_overrun(paddr_t aux_base)
{
    mfence();
    MMIO_RPI4_AUX_MU_STAT_REG reg = (MMIO_RPI4_AUX_MU_STAT_REG)(aux_base + MMIO_RPI4B_AUX_MU_STAT_REG);
    return reg->receiver_overrun;
}

static inline bool mmio_rpi4b_aux_mu_stat_reg_transmitter_full(paddr_t aux_base)
{
    mfence();
    MMIO_RPI4_AUX_MU_STAT_REG reg = (MMIO_RPI4_AUX_MU_STAT_REG)(aux_base + MMIO_RPI4B_AUX_MU_STAT_REG);
    return reg->transmitter_full;
}

static inline bool mmio_rpi4b_aux_mu_stat_reg_rts_status(paddr_t aux_base)
{
    mfence();
    MMIO_RPI4_AUX_MU_STAT_REG reg = (MMIO_RPI4_AUX_MU_STAT_REG)(aux_base + MMIO_RPI4B_AUX_MU_STAT_REG);
    return reg->rts_status;
}

static inline bool mmio_rpi4b_aux_mu_stat_reg_cts_status(paddr_t aux_base)
{
    mfence();
    MMIO_RPI4_AUX_MU_STAT_REG reg = (MMIO_RPI4_AUX_MU_STAT_REG)(aux_base + MMIO_RPI4B_AUX_MU_STAT_REG);
    return reg->cts_status;
}

static inline bool mmio_rpi4b_aux_mu_stat_reg_transmitter_empty(paddr_t aux_base)
{
    mfence();
    MMIO_RPI4_AUX_MU_STAT_REG reg = (MMIO_RPI4_AUX_MU_STAT_REG)(aux_base + MMIO_RPI4B_AUX_MU_STAT_REG);
    return reg->transmitter_empty;
}

static inline bool mmio_rpi4b_aux_mu_stat_reg_transmitter_done(paddr_t aux_base)
{
    mfence();
    MMIO_RPI4_AUX_MU_STAT_REG reg = (MMIO_RPI4_AUX_MU_STAT_REG)(aux_base + MMIO_RPI4B_AUX_MU_STAT_REG);
    return reg->transmitter_done;
}

static inline size_t mmio_rpi4b_aux_mu_stat_reg_receiver_symbol_count(paddr_t aux_base)
{
    mfence();
    MMIO_RPI4_AUX_MU_STAT_REG reg = (MMIO_RPI4_AUX_MU_STAT_REG)(aux_base + MMIO_RPI4B_AUX_MU_STAT_REG);
    return reg->receiver_symbol_count;
}

static inline size_t mmio_rpi4b_aux_mu_stat_reg_transmitter_symbol_count(paddr_t aux_base)
{
    mfence();
    MMIO_RPI4_AUX_MU_STAT_REG reg = (MMIO_RPI4_AUX_MU_STAT_REG)(aux_base + MMIO_RPI4B_AUX_MU_STAT_REG);
    return reg->transmitter_symbol_count;
}

static inline uint16_t mmio_rpi4b_aux_mu_baud_reg_rd(paddr_t aux_base)
{
    mfence();
    return *(volatile uint16_t *)(aux_base + MMIO_RPI4B_AUX_MU_BAUD_REG);
}

static inline void mmio_rpi4b_aux_mu_baud_reg_wr(paddr_t aux_base, uint16_t baudrate)
{
    mfence();
    *(volatile uint16_t *)(aux_base + MMIO_RPI4B_AUX_MU_BAUD_REG) = baudrate;
}

#endif

#endif
