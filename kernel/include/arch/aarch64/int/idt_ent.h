#ifndef KERNEL_ARCH_AARCH64_INT_IDT_ENT_H
#define KERNEL_ARCH_AARCH64_INT_IDT_ENT_H 1

#include <attributes.h>
#include <fakix/errtype.h>

typedef int ivec_t;

#define IDT_ENTSIZ 0x80
#define IVEC_MAX UINT8_MAX

typedef void(*int_callback_t)(void);

struct align(IDT_ENTSIZ) packed idt_ent {
    int_callback_t callback;
    char pad[IDT_ENTSIZ - sizeof(int_callback_t)];
};

_Static_assert(sizeof(struct idt_ent) == IDT_ENTSIZ);

static inline struct idt_ent *idt_ent_el1(void)
{
    struct idt_ent *idt;
    asm volatile ("mov %0, vbar_el1" : "=r" (idt));
    return idt;
}

static inline struct idt_ent *idt_ent_el2(void)
{
    struct idt_ent *idt;
    asm volatile ("mov %0, vbar_el2" : "=r" (idt));
    return idt;
}

static inline struct idt_ent *idt_ent_el3(void)
{
    struct idt_ent *idt;
    asm volatile ("mov %0, vbar_el3" : "=r" (idt));
    return idt;
}

errval_t idt_init(struct capability *ret_cap);
errval_t idt_set_ent(struct capability *idt, ivec_t ivec, paddr_t call, uint16_t sel);

#endif
