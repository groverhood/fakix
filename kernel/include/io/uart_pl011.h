#ifndef KERNEL_IO_UART_PL011_H
#define KERNEL_IO_UART_PL011_H 1

#include <stddef.h>
#include <stdint.h>
#include <attributes.h>

struct uart_pl011_data {
    uint32_t data : 8;
    uint32_t rest : 24;
} packed;

struct uart_pl011 {
    struct uart_pl011_data data;
    uint32_t rsrecr;
    uint8_t  pad0[16];
    uint32_t flag;
    uint8_t  pad1[4];
    uint32_t reserved;
    uint32_t ibaudrate;
    uint32_t fbaudrate;
    uint32_t lcr;
    uint32_t cntl;
    uint32_t ifls;
    uint32_t imsc;
    uint32_t ris;
    uint32_t mis;
    uint32_t icr;
    uint32_t dmacr;
    uint32_t itcr;
    uint32_t itip;
    uint32_t itop;
    uint32_t tdr;
} packed;

_Static_assert(offsetof(struct uart_pl011, flag) == 0x18, "Uh oh");

#endif