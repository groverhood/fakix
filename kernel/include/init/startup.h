#ifndef KERNEL_STARTUP_H
#define KERNEL_STARTUP_H 1

#include <stdint.h>
#include <fakix/init/bootinfo.h>

#define KAIJIMAG *(uint64_t *)((char [8]){ 'K', 'a', 'i', 'j', 'i', 'E', 'F', 'I' })

/* Attributes */
#define EFI_MEMORY_UC            0x0000000000000001
#define EFI_MEMORY_WC            0x0000000000000002
#define EFI_MEMORY_WT            0x0000000000000004
#define EFI_MEMORY_WB            0x0000000000000008
#define EFI_MEMORY_UCE           0x0000000000000010
#define EFI_MEMORY_WP            0x0000000000001000
#define EFI_MEMORY_RP            0x0000000000002000
#define EFI_MEMORY_XP            0x0000000000004000
#define EFI_MEMORY_NV            0x0000000000008000
#define EFI_MEMORY_MORE_RELIABLE 0x0000000000010000
#define EFI_MEMORY_RO            0x0000000000020000
#define EFI_MEMORY_SP            0x0000000000040000
#define EFI_MEMORY_CPU_CRYPTO    0x0000000000080000
#define EFI_MEMORY_RUNTIME       0x8000000000000000

/* Types */
enum {
    EfiReservedMemoryType,
    EfiLoaderCode,
    EfiLoaderData,
    EfiBootServicesCode,
    EfiBootServicesData,
    EfiRuntimeServicesCode,
    EfiRuntimeServicesData,
    EfiConventionalMemory,
    EfiUnusableMemory,
    EfiACPIReclaimMemory,
    EfiACPIMemoryNVS,
    EfiMemoryMappedIO,
    EfiMemoryMappedIOPortSpace
};

struct efi_memory_descriptor {
    uint32_t type;
    uintptr_t pstart;
    uintptr_t vstart;
    uint64_t npages;
    uint64_t attr;
};

struct bootstruct {
    uint64_t magic;
    uint64_t ramdisk_size;
    uint8_t *ramdisk;
    uint64_t memmap_size;
    uint64_t memmap_entsz;
    struct efi_memory_descriptor memmap[0];
};

/* Bootboot entry point */
void local_start(struct bootstruct *boot);

struct bootinfo;

/* Brief C bootstrapping function. */
void start(struct bootinfo *bi);

#endif
