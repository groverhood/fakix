#ifndef KERNEL_INT_REDIRECT_H
#define KERNEL_INT_REDIRECT_H 1

#include <stdint.h>
#include <cap/caps.h>
#include <fakix/addr.h>
#include <sys/upcall.h>
#include <int/idt_ent.h>
#include <fakix/errtype.h>

/* Interrupt redirection descriptor. Consists of the virtual address to upcall
   into. */
struct int_redirect {
    upcall_addr_t uaddr;
    tcb_handle_t tcb;
};

/* Redirect the IVT entry to upcall the provided redir address. */
errval_t int_redirect(struct capability *idt, ivec_t ivec, struct int_redirect redir);

#endif