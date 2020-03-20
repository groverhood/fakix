#include <int/redirect.h>
#include <sys/upcall.h>
#include <sys/core.h>
#include <int/isr.h>
#include <gdt.h>

static uint8_t idt_cap_buffer[VSPACE_BASE_PAGE_SIZE * 2];

struct packed idt_pointer { 
    uint16_t length;
    uint64_t base;
};

static struct int_redirect *get_iredir_base(void)
{
    return (struct int_redirect *)(idt_cap_buffer);
}

static struct idt_ent *get_idt_base(void)
{
    return (struct idt_ent *)(idt_cap_buffer + VSPACE_BASE_PAGE_SIZE);
}

#define ireturn() asm volatile("iretq")

void isr_common_stub(ivec_t ivec)
{
    struct int_redirect *redirect_table = get_iredir_base();
    tcb_handle_t cur = sys_task_on_core(sys_core_id());
    tcb_handle_t next = redirect_table[ivec].tcb;
    upcall_addr_t uaddr = redirect_table[ivec].uaddr;

    if (cur == next) {
        struct tcb_generic_shared *next_shared = tcb_get_generic_shared(next);
        next_shared->pc = uaddr;
        upcall_switch(cur, next);
    }

    ireturn();
}

static struct idt_pointer idtp asm("idtp") = {
    .length = VSPACE_BASE_PAGE_SIZE - 1,
    .base = (paddr_t)idt_cap_buffer - VSPACE_KERN_BASE + VSPACE_BASE_PAGE_SIZE
};

errval_t idt_init(struct capability *ret_cap)
{
    ret_cap->base = (paddr_t)idt_cap_buffer - VSPACE_KERN_BASE;
    ret_cap->objtype = CAP_OBJECT_IDT;
    ret_cap->rights = CAP_RIGHTS_RDWR;
    ret_cap->size = VSPACE_BASE_PAGE_SIZE * 2;
    
    asm volatile("lidt (%0)\n" :: "r" (&idtp));

    return ERR_OK;
}

errval_t idt_set_ent(struct capability *idt, ivec_t ivec, paddr_t call, uint16_t sel)
{
    errval_t err = ERR_OK;
    if (call > VSPACE_KERN_BASE) {
        err = IDT_ERR_KERNEL_ADDR;
    } else if (ivec < 0 || ivec > IVEC_MAX) {
        err = IDT_ERR_INVALID_VECTOR;
    } else {
        struct idt_ent *idt_base = (struct idt_ent *)(idt->base + VSPACE_KERN_BASE + VSPACE_BASE_PAGE_SIZE);
        idt_base[ivec].cs_selector  = sel;
        idt_base[ivec].offset_15_0  = (uint16_t)(call & 0xFFFF);
        idt_base[ivec].offset_31_16 = (uint16_t)((call & 0xFFFF0000) >> 16);
        idt_base[ivec].offset_63_32 = (uint32_t)((call & 0xFFFFFFFF00000000) >> 32); 
    }

    return err;
}

errval_t int_redirect(struct capability *idt, ivec_t ivec, struct int_redirect redir)
{
    errval_t err = ERR_OK;
    if (ivec < 0 || ivec > IVEC_MAX) {
        err = IDT_ERR_INVALID_VECTOR;
    } else {
        struct int_redirect *redir_base = (struct int_redirect *)(idt->base);
        redir_base[ivec] = redir;
    }

    return err;
}

