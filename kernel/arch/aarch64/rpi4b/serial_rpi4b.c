#include <log/serial.h>
#include <io/mmio.h>

errval_t serial_init(void)
{
    /* TODO: program GPIO pins 14 and 15 to TxD and RxD, respectively (ALT0) */
}

void serial_putc(int c)
{
    while (!mmio_rpi4b_aux_mu_stat_reg_space_available(MMIO_RPI4B_AUX_BASE));
    mmio_rpi4b_aux_mu_io_reg_wr(MMIO_RPI4B_AUX_BASE, (uint8_t)c);
}

int serial_getc(void)
{
    while (!mmio_rpi4b_aux_mu_stat_reg_symbol_available(MMIO_RPI4B_AUX_BASE));
    return (int)mmio_rpi4b_aux_mu_io_reg_rd(MMIO_RPI4B_AUX_BASE);
}