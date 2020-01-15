
/* Check this header file for informative comments. */
#include <load.h>

SECTIONS
{
    fakix_boot = FAKIX_BOOT;

    fakix_real_stack = FAKIX_REAL_MODE_STACK;

    . = FAKIX_LOADER;
    .load : {
        *(.load.load_entry);
        *(.load);
    }

    fakix_image_header = FAKIX_IMAGE_HEADER;

    fakix_memmap = FAKIX_MEMMAP;

    fakix_rsdt = FAKIX_RSDT;

    fakix_pr_stack = FAKIX_PROT_MODE_STACK;

    fakix_boot_pml4 = FAKIX_BOOT_PML4;
    fakix_boot_pdp = FAKIX_BOOT_PDP;
    fakix_boot_pgdir = FAKIX_BOOT_PGDIR;
    fakix_boot_pgtbl = FAKIX_BOOT_PGTBL;

    fakix_kern_pml4 = FAKIX_KERN_PML4;
    fakix_kern_pdp = FAKIX_KERN_PDP;
    fakix_kern_pgdir = FAKIX_KERN_PGDIR;
    fakix_kern_pgtbl = FAKIX_KERN_PGTBL;

    fakix_kern_start = FAKIX_KERN_START;
}