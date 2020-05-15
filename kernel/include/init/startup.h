#ifndef KERNEL_STARTUP_H
#define KERNEL_STARTUP_H 1

#include <stdint.h>
#include <fakix/init/bootinfo.h>

#define KAIJIMAG *(uint64_t *)({ 'K', 'a', 'j', 'i', 'E', 'F', 'I' })

struct bootstruct {
    uint64_t magic;
    uint64_t ramdisk_size;
    uint8_t ramdisk[0];
};

/* Bootboot entry point */
void _start(struct bootstruct *boot);

/* Brief C bootstrapping function. */
void start(struct bootinfo *bi);

#endif
