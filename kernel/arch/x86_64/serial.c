
#include <io/portio.h>

#define SCOMM_PORT 0x3F8

#define LSR_OFFSET 5
#define LSR_TRANSMITTER_EMPTY_BIT (1 << 6)
#define LSR_DATA_AVAILABLE_BIT (1 << 0)

errval_t serial_init(void)
{
    return ERR_OK;
}

static void transmitter_spin(portid_t port)
{
    uint8_t b;
    while (err_is_ok(inb(port + LSR_OFFSET, &b)) && (b & LSR_TRANSMITTER_EMPTY_BIT) == 0);
}

static void data_spin(portid_t port)
{
    uint8_t b;
    while (err_is_ok(inb(port + LSR_OFFSET, &b)) && (b & LSR_DATA_AVAILABLE_BIT) == 0);
}

void serial_putc(int c)
{
    transmitter_spin(SCOMM_PORT);
    outb(SCOMM_PORT, c);
}

int serial_getc(void)
{
    uint8_t b;
    data_spin(SCOMM_PORT);
    return err_is_ok(inb(SCOMM_PORT, &b)) ? b : -1;
}
