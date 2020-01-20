#ifndef FAKIX_H
#define FAKIX_H 1

#include <stdint.h>
#include <attributes.h>

struct fakix_image_header {
    uint64_t magic;
    uint64_t image_size;
    uint64_t kern_block_start;
    uint64_t kern_block_size;
    uint64_t fs_block_start;
    uint64_t fs_block_size;
    uint64_t swap_block_start;
    uint64_t swap_block_size;
    uint64_t boot_block_start;
    uint64_t boot_block_size;
};

struct fakix_memmap {
    uint64_t base_address;
    uint64_t length;
    uint32_t type;
    uint32_t acpi_extented_attrs;
};

packed struct fakix_sdt {
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oem_id[6];
    char oem_table_id[8];
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
};

struct fakix_system_table {
    /* Memory map base pointer. */
    struct fakix_memmap *mmap;
    struct fakix_image_header *imghdr;
    struct fakix_sdt *rsdt;
};

#endif