/**
 * === load.h ===
 * 
 * This header file contains only preprocessor directives, which allows it to
 * be used in ASM/C sources as well as linker scripts, allowing for more 
 * uniform and by extension easily debugable code.
 * 
 **/

#ifndef FAKIX_ARCH_X86_64_LOAD_H
#define FAKIX_ARCH_X86_64_LOAD_H 1

/* Maximum size of stacks used in FAKIX. */
#define FAKIX_STACK_SIZE 0x1000

/* Where we load the rest of the Boot Module. */
#define FAKIX_BOOT 0x1000
#define FAKIX_BOOT_SIZE 0x2400

/* The location of the stack used in real mode. */
#define FAKIX_REAL_MODE_STACK 0x7000 

/* Where the loader is stored in physical memory. */
#define FAKIX_LOADER 0x7C00
#define FAKIX_LOADER_SIZE 0x200

/* Where the FAKIX image header is loaded. */
#define FAKIX_IMAGE_HEADER 0x8000
#define FAKIX_IMAGE_HEADER_SIZE 0x200

/* Where the FAKIX memory map is loaded. */
#define FAKIX_MEMMAP 0x8200
#define FAKIX_MEMMAP_SIZE 0x600

/* Where the FAKIX root system descriptor table is loaded. */
#define FAKIX_RSDT 0x8800
#define FAKIX_RSDT_SIZE 0x100

/* Location of the stack used in protected mode. */
#define FAKIX_PROT_MODE_STACK 0xE000

/* Upon entering fakix_start, we immediately switch to this page table. */
#define FAKIX_KERN_PML4  0x10000
#define FAKIX_KERN_PDP   0x11000
#define FAKIX_KERN_PGDIR 0x12000
#define FAKIX_KERN_PGTBL 0x13000

/* We will only map the first 2M of this page table to prevent triple faulting
   into oblivion when we enable paging in code that uses physical addresses. */
#define FAKIX_BOOT_PML4  0x14000
#define FAKIX_BOOT_PDP   0x15000
#define FAKIX_BOOT_PGDIR 0x16000
#define FAKIX_BOOT_PGTBL 0x17000

/* The physical address of the FAKIX kernel. */
#define FAKIX_KERN_START 0x18000

/* The maximum size of the FAKIX kernel, rounded down to the nearest K. */
#define FAKIX_KERN_SIZE (((0x9FC00) - FAKIX_KERN_START) & ~0x3FF)


#endif
