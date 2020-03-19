#ifndef KERNEL_ARCH_X86_64_GDT_H
#define KERNEL_ARCH_X86_64_GDT_H 1

#include <attributes.h>
#include <fakix/addr.h>
#include <stdint.h>

packed struct gdt_descriptor {
    uint8_t unused0[5]; /* Unused when paging is enabled. */
    uint8_t access;
    uint8_t flags;
    uint8_t unused1; /* Unused when paging is enabled. */
};

#define GDT_SEGMENT_PRESENT 0x80
#define GDT_PRIVILEGE_RING0 0x00
#define GDT_PRIVILEGE_RING1 0x20
#define GDT_PRIVILEGE_RING2 0x40
#define GDT_PRIVILEGE_RING3 0x60

#define GDT_TYPE_DATA 0x10
#define GDT_TYPE_DATA_RDWR 0x12
#define GDT_TYPE_CODE 0x18
#define GDT_TYPE_CODE_READ 0x1A

#define GDT_FLAGS_CODE_64 0x20

#define GDT_KERNEL_CODE_SEL 0x08
#define GDT_KERNEL_DATA_SEL 0x10
#define GDT_USER_CODE_SEL 0x18
#define GDT_USER_DATA_SEL 0x20

#define GDT_KERNEL_CODE_64 ((struct gdt_descriptor){ \
        .access = GDT_SEGMENT_PRESENT | GDT_PRIVILEGE_RING0 | GDT_TYPE_CODE_READ,\
        .flags = GDT_FLAGS_CODE_64 \
    })

#define GDT_KERNEL_DATA_64 ((struct gdt_descriptor){ \
        .access = GDT_SEGMENT_PRESENT | GDT_PRIVILEGE_RING0 | GDT_TYPE_DATA_RDWR,\
        .flags = 0 \
    })

#define GDT_USER_CODE_64 ((struct gdt_descriptor){ \
        .access = GDT_SEGMENT_PRESENT | GDT_PRIVILEGE_RING3 | GDT_TYPE_CODE_READ,\
        .flags = GDT_FLAGS_CODE_64 \
    })

#define GDT_USER_DATA_64 ((struct gdt_descriptor){ \
        .access = GDT_SEGMENT_PRESENT | GDT_PRIVILEGE_RING3 | GDT_TYPE_DATA_RDWR,\
        .flags = 0 \
    })

#define GDT_NENTS 4

packed struct gdt {
    struct gdt_descriptor null_sel;
    struct gdt_descriptor descriptors[GDT_NENTS];
};

packed struct gdt_pointer {
    uint16_t size;
    paddr_t gdt_addr;
};

#endif