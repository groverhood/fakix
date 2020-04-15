#ifndef FAKIX_TCB_H
#define FAKIX_TCB_H 1

#include <stdbool.h>
#include <fakix/addr.h>
#include <fakix/cspace.h>
#include <fakix/paging.h>
#include <cpu/registers.h>
#include <cpu/fpu_state.h>
#include <fakix/tcb_arch.h>

#define TASK_BITS 16

typedef vaddr_t tcb_handle_t;

struct thread;

/* Shared user/kernel TCB struct */
struct tcb_generic_shared {
    union registers disabled_save_area, enabled_save_area;
    struct fpu_state fpu_state;
    bool fpu_trap;
    bool disabled;
    struct tcb_arch_shared arch_state;
    vaddr_t pc;
};

/* User-only TCB struct */
struct tcb_generic {
    struct tcb_generic_shared shared;
    struct tcb_arch arch_state;

    struct paging_state *pager;
    vaddr_t self;
    struct thread *current;
};

static inline struct tcb_generic *tcb_get_generic(tcb_handle_t tcb)
{
    return (struct tcb_generic *)tcb;
}

static inline struct tcb_generic_shared *tcb_get_generic_shared(tcb_handle_t tcb)
{
    return (struct tcb_generic_shared *)tcb;
}

static inline struct tcb_arch *tcb_get_arch(tcb_handle_t tcb)
{
    return &tcb_get_generic(tcb)->arch_state;
}

static inline struct tcb_arch_shared *tcb_get_arch_shared(tcb_handle_t tcb)
{
    return &tcb_get_arch(tcb)->shared;
}

#endif