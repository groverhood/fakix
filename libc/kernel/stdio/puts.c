#include <io/risky.h>
#include <io/serial.h>
#include <stdio.h>
#include <string.h>

int puts(const char *s)
{
    if (serial_initialized) {

    } else {
        risky_puts(s);
    }

    return (int)strlen(s);
}