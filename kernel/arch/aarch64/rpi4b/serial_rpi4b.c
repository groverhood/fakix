#include <io/mmio_rpi4b_gpu.h>
#include <io/uart_pl011.h>
#include <log/serial.h>
#include <io/mmio.h>

#define BAUDRATE 115200

errval_t serial_init(void)
{
    volatile struct mmio_rpi4b_gpio *gpio = (volatile struct mmio_rpi4b_gpio *)(MMIO_RPI4B_GPIO_BASE);
    volatile struct uart_pl011 *uart0 = (volatile struct uart_pl011 *)(MMIO_RPI4B_UART0_BASE);

    gpio->gpfsel1.fsel14 = MMIO_RPI4B_GPIO_FSEL_ALT0;
    gpio->gpfsel1.fsel15 = MMIO_RPI4B_GPIO_FSEL_ALT0;

    gpio->ppc0.gpio14 = 0;
    gpio->ppc0.gpio15 = 0;

    uart0->icr = 0x7ff;

    struct mmio_rpi4b_gpu_msg_hdr msg =
        { GPU_MSG_HDR_SIZ + 3 * sizeof(uint32_t), 0, GPU_CMD_GET_CLOCK_RATE, 8, 4,
          .argv = { GPU_CLOCK_UART, 0, 0 } };

    mmio_rpi4b_gpu_mbox_write(GPU_CHANNEL_TAGS, &msg);
    mmio_rpi4b_gpu_mbox_read(GPU_CHANNEL_TAGS);
    asm volatile ("brk #0");
    uint32_t uartfreq = msg.argv[1];
    uint32_t ibaud = uartfreq / (16 * BAUDRATE);
    uint32_t fbaud = (uint32_t)(((float)uartfreq / (16.0f * BAUDRATE)) - ibaud * 64.0f + 0.5f);

    uart0->ibaudrate = ibaud;
    uart0->fbaudrate = fbaud;
    uart0->lcr = (0x3 << 5);

    mfence();
    uart0->cntl = (1 << 9) | (1 << 8) | (1 << 0);

    return ERR_OK;
}

void serial_putc(int c)
{
    volatile struct uart_pl011 *uart0 = (volatile struct uart_pl011 *)(MMIO_RPI4B_UART0_BASE);
    while ((uart0->flag & (1 << 5)) == 1);
    mfence();
    uart0->data.data = c;
}

int serial_getc(void)
{
    volatile struct uart_pl011 *uart0 = (volatile struct uart_pl011 *)(MMIO_RPI4B_UART0_BASE);
    while ((uart0->flag & (1 << 3)) != 0);
    mfence();
    return (int)uart0->data.data;
}
