
#include <load.h>

SECTIONS
{
    /* This must be defined for all architectures. */
    . = FAKIX_KERNEL_START;

    .text ALIGN(0x1000) : AT(ADDR(.text) - 0xFFFF800000000000) {
        *(.text.fakix_start);
        *(.text);
        _etext = ABSOLUTE(.);
    }

    .data ALIGN(0x1000) : AT(ADDR(.text) - 0xFFFF800000000000) {
        *(.data);
        _edata = ABSOLUTE(.);
    }

    .bss ALIGN(0x1000) : AT(ADDR(.bss) - 0xFFFF800000000000) {
        *(.bss);
        _end = ABSOLUTE(.);
    }
}