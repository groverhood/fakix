
#include <log/print.h>
#include <log/serial.h>

void kernel_log(const char *msg)
{
    while (*msg) {
        serial_putc(*msg++);
    }
}