
#include <fakix/vspace.h>

ENTRY(_start)
SECTIONS
{
    /* This must be defined for all architectures. */
    mmio = VSPACE_MMIO_BASE; 
    fb = VSPACE_FB_BASE;
    . = VSPACE_KERN_BASE;

    bootboot = .; . += VSPACE_BASE_PAGE_SIZE;
    environment = .; . += VSPACE_BASE_PAGE_SIZE;

    .text ALIGN(4K) : {
        *(.text);
        _etext = ALIGN(4K);
    }

    .rodata ALIGN(4K) : {
        *(.rodata);
    }

    .data ALIGN(4K) : {
        *(.data);
        _edata = ABSOLUTE(.);
    }
    .bss ALIGN(4K) : {
        *(.bss);
        _end = ABSOLUTE(.);
    }
}