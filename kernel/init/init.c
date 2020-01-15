
#include <fakix.h>
#include <arch/compat.h>
#include <paging/table.h>
#include <paging/conv.h>

extern int fakix_main(struct fakix_system_table *systable);

void fakix_start(void)
{
    /* After this, boot code will no longer be accessible. */
    paging_table_set_to(FAKIX_KERN_PAGE_TABLE);

    struct fakix_system_table systable = {
        conv_phys_to_kern(FAKIX_KERNEL_MMAP),
        conv_phys_to_kern(FAKIX_KERNEL_IMAGE_HEADER),
        conv_phys_to_kern(FAKIX_KERNEL_RSDT)
    };

    if (fakix_main(&systable) != 0) {
        
    }
}