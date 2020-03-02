#include "mm.h"
#include <fakix/ram.h>
#include <fakix/init/ramcaps.h>


void mm_init(void)
{
    ram_set_alloc_func(&mm_alloc_aligned);
}

void mm_alloc_aligned(size_t size, size_t alignment, capaddr_t *ret_cap)
{
    static capaddr_t ram_bptr = CAP_INIT_RAM_BASE; 
}