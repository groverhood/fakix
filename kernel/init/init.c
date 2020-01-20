
#include <stdbool.h>
#include <fakix.h>
#include <arch/compat.h>
#include <paging/table.h>
#include <paging/conv.h>

extern int fakix_main(struct fakix_system_table *systable);

#define hang() \
    do { \
        __asm__ __volatile__ ("nop");\
    } while (true)

void fakix_start(void)
{
    /* After this, boot code will no longer be accessible. */
    paging_table_init(FAKIX_KERN_PAGE_TABLE);

    void *mmap = FAKIX_KERNEL_MMAP;
    void *imghdr = FAKIX_KERNEL_IMAGE_HEADER;
    uintptr_t *prsdt = FAKIX_KERNEL_RSDT;

    struct fakix_system_table systable = {
        mmap,
        imghdr,
        conv_phys_to_kern(*prsdt)
    };

    if (fakix_main(&systable) != 0) {
        /* Handle fatal kernel error... */
    }

    /* For now, just loop forever... */
    hang();
}