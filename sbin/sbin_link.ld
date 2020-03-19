#include <fakix/vspace.h>

SECTIONS
{
    . = VSPACE_GAP_TOP;

    .text ALIGN(4K) : AT(ADDR(.text) - VSPACE_GAP_TOP) { 
        *(.text);
    }
    _etext = .;

    .rodata ALIGN(4K) : AT(ADDR(.rodata) - VSPACE_GAP_TOP) {
        *(.rodata);
    }

    .data ALIGN(4K) : AT(ADDR(.data) - VSPACE_GAP_TOP) {
        *(.data);
    }
    _edata = .;

    .bss ALIGN(4K) : AT(ADDR(.bss) - VSPACE_GAP_TOP) {
        *(.bss);
    }
    _end = .;
}