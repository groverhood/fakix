
#include <multiboot/multiboot2.h>
#include <fakix/vspace.h>

.section .multiboot
.extern start
.extern _btext
.extern _etext
.extern _end

multiboot_header_start:
multiboot_header:
    .long MULTIBOOT2_HEADER_MAGIC
    .long MULTIBOOT_ARCHITECTURE_I386
    .long (multiboot_header_end - multiboot_header_start)
    .long -(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT_ARCHITECTURE_I386 + (multiboot_header_end - multiboot_header_start))
multiboot_address_tag:
    .long MULTIBOOT_HEADER_TAG_ADDRESS
    .long 24
    .long (multiboot_header - VSPACE_KERN_BASE)
    .long (_btext - VSPACE_KERN_BASE)
    .long (_etext - VSPACE_KERN_BASE)
    .long (_end - VSPACE_KERN_BASE)
multiboot_entry_tag:
    .long MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS
    .long 12
    .long (start - VSPACE_KERN_BASE)
multiboot_info_request:
    .word MULTIBOOT_HEADER_TAG_INFORMATION_REQUEST
    .word MULTIBOOT_HEADER_TAG_OPTIONAL
    .long 20
    .long MULTIBOOT_TAG_TYPE_MODULE
    .long MULTIBOOT_TAG_TYPE_MMAP
    .long MULTIBOOT_TAG_TYPE_ACPI_NEW
multiboot_end_tag:
    .long MULTIBOOT_HEADER_TAG_END
    .long 8
multiboot_header_end: