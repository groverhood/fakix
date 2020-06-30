#include <sys/core.h>
#include <log/serial.h>
#include <log/print.h>
#include <init/startup.h>
#include <fakix/init/bootinfo.h>

char print_buffer[VSPACE_BASE_PAGE_SIZE];
static char stack_base[VSPACE_BASE_PAGE_SIZE];

#define SPIN() do { asm (""); } while (1)

void local_start(struct bootstruct *boot)
{
    //size_t offset;
    /* uintptr_t memmap_base = (uintptr_t)boot->memmap; */
    /* for (offset = 0; offset < boot->memmap_size; offset += boot->memmap_entsz) { */
    /*     struct efi_memory_descriptor *mem = (struct efi_memory_descriptor *)(memmap_base + offset); */
    /*     switch (mem->type) { */
    /*         case EfiMemoryMappedIO: mmio_base = mem->pstart; break; */
    /*     } */
    /* } */

    asm volatile ("msr SP_EL0, %0\n\t"
                  "msr SPSel, #0" :: "r" (stack_base + sizeof stack_base));

    /* serial_init(); */
    serial_putc('a');

    SPIN();
}
