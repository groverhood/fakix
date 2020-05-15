#ifndef KAIJI_BOOTSTRUCT_H
#define KAIJI_BOOTSTRUCT_H 1

#include <elf.h>
#include <stdint.h>

typedef uint8_t  bootbyte_t;
typedef uint16_t boothalf_t;
typedef uint32_t bootword_t;
typedef uint64_t bootdouble_t;

#define BOOTMAGIC *(bootdouble_t *)((char[8]){ 'K', 'a', 'i', 'j', 'i', 'E', 'F', 'I' })
#define BOOTMAXMACHID 255

struct bootstruct {
    bootdouble_t magic;
    bootdouble_t size;
    bootbyte_t ramdisk[0];
} __attribute__((packed));

#endif
