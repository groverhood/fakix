#include <fakix.h>
#include <io/dev.h>
#include <io/serial.h>
#include <paging/alloc.h>
#include <paging/table.h>
#include <stdio.h>

int fakix_main(struct fakix_system_table *systable)
{
    init_devices(systable->rsdt);
    init_serial();
    init_page_alloc(systable->mmap);

    return 0;
}