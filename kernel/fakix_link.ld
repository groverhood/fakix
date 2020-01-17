
#include <load.h>

SECTIONS
{
    /* This must be defined for all architectures. */
    . = FAKIX_KERN_START + 0xFFFF800000000000;

    .text ALIGN(4K) : AT(ADDR(.text) - 0xFFFF800000000000) {
        *(.text.fakix_start);
        *(.text);
        _etext = ABSOLUTE(.);
    }

    .rodata ALIGN(4K) : AT(ADDR(.text) - 0xFFFF800000000000) {
        *(.rodata);
    }

    .data ALIGN(4K) : AT(ADDR(.data) - 0xFFFF800000000000) {
        *(.data);
        _edata = ABSOLUTE(.);
    }

    .bss ALIGN(4K) : AT(ADDR(.bss) - 0xFFFF800000000000) {
        *(.bss);
        _end = ABSOLUTE(.);
    }
}