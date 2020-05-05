#ifndef KAIJI_BOOTSTRUCT_H
#define KAIJI_BOOTSTRUCT_H 1

#include <elf.h>
#include <stdint.h>

typedef uint8_t  bootbyte_t;
typedef uint16_t boothalf_t;
typedef uint32_t bootword_t;
typedef uint64_t bootdouble_t;

#define BOOTMAGIC 0x746f6f62
#define BOOTMAXMACHID 255

struct bootstruct {
    bootword_t magic;
    bootword_t mach_id;
    bootdouble_t size;
    bootbyte_t ramdisk[0];
} __attribute__((packed));

#endif