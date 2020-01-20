#ifndef FAKIX_IO_SERIAL_H
#define FAKIX_IO_SERIAL_H 1

#include <stdbool.h>
#include <stddef.h>
#include <io/dev.h>

extern bool serial_initialized;

void init_serial(void);

#endif