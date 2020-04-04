#ifndef KERNEL_STARTUP_H
#define KERNEL_STARTUP_H 1

#include <fakix/init/bootinfo.h>

/* Bootboot entry point */
void _start(void);

/* Brief C bootstrapping function. */
void start(struct bootinfo *bi);

#endif