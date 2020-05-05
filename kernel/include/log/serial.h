#ifndef KERNEL_LOG_SERIAL_H
#define KERNEL_LOG_SERIAL_H 1

#include <fakix/errtype.h>

errval_t serial_init(void);
void serial_putc(int c);
int serial_getc(void);

#endif