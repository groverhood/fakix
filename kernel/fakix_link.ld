
#include <fakix/vspace.h>

SECTIONS
{

    /* This must be defined for all architectures. */
    . = VSPACE_KERN_BASE + VSPACE_KERN_OFFSET;

    .text ALIGN(4K) : AT(ADDR(.text) - VSPACE_KERN_BASE) {
        _btext = ABSOLUTE(.);
        *(.text);
        _etext = ABSOLUTE(.);
    }

    .rodata ALIGN(4K) : AT(ADDR(.rodata) - VSPACE_KERN_BASE) {
        *(.rodata);
    }

    .data ALIGN(4K) : AT(ADDR(.data) - VSPACE_KERN_BASE) {
        *(.data);
        _edata = ABSOLUTE(.);
    }

    .bss ALIGN(4K) : AT(ADDR(.bss) - VSPACE_KERN_BASE) {
        *(.bss);
        _end = ABSOLUTE(.);
    }
}