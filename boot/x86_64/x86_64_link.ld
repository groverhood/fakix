
INCLUDE x86_64_load.h

SECTIONS
{
    /* Where we load the rest of the Boot Module. 9K (0x2400) */
    fakix_boot = 0x1000;

    fakix_real_stack = 0x7000;
    . = 0x7C00;

    .load : {
        *(.load);
    }

    /* 512 bytes (0x200) */
    fakix_image_header = 0x8000;

    /* 2.5K (0x600) */
    fakix_memmap = 0x8200;

    /* 256 bytes (0x100) */
    fakix_rsdt = 0x8800;

    /* 4K (0x1000) */
    fakix_pr_stack = 0xE000;

    video_mmio_bios = 0xF0000;
}