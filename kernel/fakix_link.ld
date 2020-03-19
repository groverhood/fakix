
#include <elf/format.h>
#include <fakix/vspace.h>

ENTRY(_start)
SECTIONS
{
    . = VSPACE_KERN_OFFSET;
    .start ALIGN(4K) : AT(VSPACE_KERN_OFFSET) {
        *(.multiboot);
        *(.start);
        *(.start.*);
    }

    /* This must be defined for all architectures. */
    . += VSPACE_KERN_BASE;

    .text ALIGN(4K) : AT(ADDR(.text) - VSPACE_KERN_BASE) {
        *(.text);
        _etext = ALIGN(4K);
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