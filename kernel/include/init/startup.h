#ifndef KERNEL_STARTUP_H
#define KERNEL_STARTUP_H 1

#include <multiboot/multiboot2.h>

struct multiboot_info {
    multiboot_uint32_t size;
    multiboot_uint32_t rsvd;
    struct multiboot_header_tag tags[0];
};

struct generic_init_info {
    struct capability *l1_cnode;
    struct multiboot_info *boot_info;
};

/* Multiboot entry point */
void _start(void);

/* Brief C bootstrapping function. */
void start(void);

/* Architecture-specific init. */
void local_init(multiboot_uint32_t magic, struct multiboot_info *boot_info);

/* Architecture-specific init. */
void init(struct multiboot_info *boot_info);

#endif