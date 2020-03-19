#include <sys/upcall.h>
#include <fakix/errors.h>

/* Perform an upcall into the provided user address. */
errval_t upcall_jump(upcall_addr_t uaddr, regval_t flags)
{
    asm volatile ("movq %[FLAGS], %%r11; sti; sysret" :: "c" (uaddr), [FLAGS] "D" (flags));
    return ERR_OK; /* Not reached. */
}

static inline errval_t tcb_switch(tcb_handle_t cur, tcb_handle_t next)
{
    return ERR_OK;
}

/* Perform a context switch and an upcall. */
errval_t upcall_switch(tcb_handle_t cur, tcb_handle_t next)
{
    errval_t err = tcb_switch(cur, next);
    if (err_is_ok(err)) {
        struct tcb_generic_shared *sg = tcb_get_generic_shared(next);
        struct tcb_arch_shared *sa = tcb_get_arch_shared(next);
        err = upcall_jump(sg->pc, sa->flags);
    }

    return err;
}