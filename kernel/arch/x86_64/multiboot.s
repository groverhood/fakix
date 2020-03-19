
#define ASM_FILE 1

#include <multiboot/multiboot2.h>
#include <fakix/vspace.h>

.section .multiboot
.extern _start
.extern _edata
.extern _end

.globl multiboot_header

multiboot_header_start:
multiboot_header:
    .long MULTIBOOT2_HEADER_MAGIC
    .long MULTIBOOT_ARCHITECTURE_I386
    .long (multiboot_header_end - multiboot_header_start)
    .long -(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT_ARCHITECTURE_I386 + (multiboot_header_end - multiboot_header_start))
multiboot_address_tag:
    .word MULTIBOOT_HEADER_TAG_ADDRESS
    .word 0
    .long (multiboot_entry_tag - multiboot_address_tag)
    .long multiboot_header
    .long _start
    .long (_edata - VSPACE_KERN_BASE)
    .long (_end - VSPACE_KERN_BASE)
multiboot_entry_tag:
    .word MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS
    .word 0
    .long (multiboot_info_request - multiboot_entry_tag)
    .long _start
multiboot_info_request:
    .word MULTIBOOT_HEADER_TAG_INFORMATION_REQUEST
    .word MULTIBOOT_HEADER_TAG_OPTIONAL
    .long (multiboot_end_tag - multiboot_info_request)
    .long MULTIBOOT_TAG_TYPE_MODULE
    .long MULTIBOOT_TAG_TYPE_MMAP
    .long MULTIBOOT_TAG_TYPE_ACPI_NEW
multiboot_end_tag:
    .long MULTIBOOT_HEADER_TAG_END
    .long 8
multiboot_header_end: