#include <sys/upcall.h>
#include <fakix/errors.h>

/* Perform an upcall into the provided user address. */
errval_t upcall_jump(upcall_addr_t uaddr)
{
    asm volatile (
        "pushfq\n\t"
        "popq %%r11\n\t" 
        "sti\n\t"
        "sysret" 
        :: "c" (uaddr)
    );

    return ERR_OK; /* Not reached. */
}