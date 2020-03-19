#ifndef KERNEL_ARCH_X86_64_INT_IDT_ENT_H
#define KERNEL_ARCH_X86_64_INT_IDT_ENT_H 1

#include <fakix/errtype.h>
#include <attributes.h>
#include <cap/caps.h>
#include <stdint.h>

typedef int ivec_t;

#define IVEC_MAX UINT8_MAX

typedef uint8_t ist_ring_t;
typedef uint8_t idt_ent_attr_t;

packed struct idt_ent {
    uint16_t offset_15_0;
    uint16_t cs_selector;
    ist_ring_t ist;
    idt_ent_attr_t attrs;
    uint16_t offset_31_16;
    uint32_t offset_63_32;
    uint32_t rsvd;
};

errval_t idt_init(struct capability *ret_cap);
errval_t idt_set_ent(struct capability *idt, ivec_t ivec, paddr_t call, uint16_t sel);

#endif