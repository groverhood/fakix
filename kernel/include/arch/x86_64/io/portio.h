#ifndef KERNEL_ARCH_X86_64_PORTIO_H
#define KERNEL_ARCH_X86_64_PORTIO_H 1

#include <stdint.h>
#include <stddef.h>
#include <fakix/errors.h>
#include <fakix/errtype.h>

typedef uint16_t portid_t;

static inline errval_t inb(portid_t port, uint8_t *b)
{
    if (b == NULL) {
        return KERNEL_ERR_PORTIO_NULL_RETPTR;
    }

    asm volatile ("inb %%dx, %%al" : "=a" (*b) : "d" (port));

    return ERR_OK;
}

static inline errval_t outb(portid_t port, uint8_t b)
{
    asm volatile ("outb %%al, %%dx" :: "a" (b), "d" (port));
}

#endif