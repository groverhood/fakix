#include <io/uart_pl011.h>
#include <log/serial.h>
#include <io/mmio.h>

errval_t serial_init(void)
{
    /* UEFI should already have done this initialization... */
    volatile struct mmio_rpi4b_gpio *gpio = (struct mmio_rpi4b_gpio *)MMIO_RPI4B_GPIO_BASE;
    volatile struct mmio_rpi4b_gpio_gpfsel1 *gpfsel1 = &gpio->gpfsel1;
    gpfsel1->fsel14 = MMIO_RPI4B_GPIO_FSEL_ALT0;
    gpfsel1->fsel15 = MMIO_RPI4B_GPIO_FSEL_ALT0;
}

void serial_putc(int c)
{
    volatile struct uart_pl011 *uart0 = (struct uart_pl011 *)MMIO_RPI4B_UART0_BASE;
    while ((uart0->flag & (1 << 7)) == 0);
    uart0->data &= (~0xff | (c & 0xff));
}

int serial_getc(void)
{
    volatile struct uart_pl011 *uart0 = (struct uart_pl011 *)MMIO_RPI4B_UART0_BASE;
    while ((uart0->flag & (1 << 3)) != 0);
    return (int)uart0->data & 0xff;
}
