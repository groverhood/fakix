#include <apic.h>
#include <attributes.h>
#include <io/interrupt.h>
#include <paging/conv.h>
#include <proc/timer.h>
#include <util/panic.h>
#include <stdint.h>

#define IA32_APIC_BASE_MSR 0x1B
#define IA32_APIC_BASE_MSR_BSP (1 << 8)
#define IA32_APIC_BASE_MSR_ENABLE (1 << 11)

static void interrupt_init_idt(void);

void interrupt_init(void)
{
    interrupt_init_idt();

    /* Enable the Local APIC. */

    uint64_t ia32_apic_base_msr;
    __asm__ volatile("rdmsr\n\t"
                     "orq %%rdx, %0"
                     : "=a"(ia32_apic_base_msr)
                     : "c"(IA32_APIC_BASE_MSR)
                     : "rdx");

    ia32_apic_base_msr |= (IA32_APIC_BASE_MSR_BSP | IA32_APIC_BASE_MSR_ENABLE);

    __asm__ volatile("wrmsr"
                     :
                     : "a"(ia32_apic_base_msr), "d"(ia32_apic_base_msr >> 32),
                       "c"(IA32_APIC_BASE_MSR));
}

packed struct idt_pointer {
    uint16_t limit; /* Length of IDT in bytes - 1. */
    uint64_t base;  /* Physical address of the IDT. */
};

packed struct idt_descriptor {
    uint16_t ofs_15_0;
    uint16_t cs_selector;
    uint8_t rsvd0;
    uint8_t type_attr;
    uint16_t ofs_31_16;
    uint32_t ofs_63_32;
    uint32_t rsvd1;
};

/* Align the IDT to a page boundary so we can describe it with capabilities. */
align(FAKIX_PAGE) struct idt_descriptor idt[INTRCODE_MAX + 1];

static interrupt_handler_t div_error_handler;
static interrupt_handler_t invalid_opcode_handler;
static interrupt_handler_t double_fault_handler;
static interrupt_handler_t gp_fault_handler;

static void interrupt_init_idt(void)
{
    interrupt_set_handler(INTRCODE_DIVIDE_ERROR, &div_error_handler);
    interrupt_set_handler(INTRCODE_INVALID_OPCODE, &invalid_opcode_handler);
    interrupt_set_handler(INTRCODE_DOUBLE_FAULT, &double_fault_handler);
    interrupt_set_handler(INTRCODE_GENERAL_PROTECTION_FAULT, &gp_fault_handler);

    struct idt_pointer idtp = {.limit = sizeof idt + 1, (uintptr_t)idt};

    __asm__ volatile("lidt (%[IDTP])" ::[IDTP] "r"(&idtp));
}

void interrupt_set_handler(intrcode_t intr, interrupt_handler_t *handler)
{
    struct idt_descriptor *idt_desc = (idt + intr);
    uintptr_t handler_paddr = conv_kern_to_phys(handler);

    idt_desc->ofs_15_0 = (uint16_t)(handler_paddr);
    idt_desc->ofs_31_16 = (uint16_t)(handler_paddr >> 16);
    idt_desc->ofs_63_32 = (uint32_t)(handler_paddr >> 32);
    idt_desc->cs_selector = 8;
    idt_desc->type_attr = 0x8E;
}

void div_error_handler(struct interrupt_state *intr_state)
{
    panic("DIV error");
    ireturn;
}

void invalid_opcode_handler(struct interrupt_state *intr_state)
{
    panic("Invalid opcode");
    ireturn;
}

void double_fault_handler(struct interrupt_state *intr_state)
{
    panic("Double Fault");
    ireturn;
}

void gp_fault_handler(struct interrupt_state *intr_state)
{
    panic("GP Fault");
    ireturn;
}

void timer_init(void)
{
    __asm__ volatile("outb %%al, %%dx" ::"d"(0x21), "a"(0xFF));
    __asm__ volatile("outb %%al, %%dx" ::"d"(0x91), "a"(0xFF));

    *LAPIC_SPIRV_REGISTER &= ~0xFF;
    *LAPIC_SPIRV_REGISTER |= INTRCODE_SPIRV_INTERRUPT;

    *LAPIC_LVT_TIMER_REGISTER &= ~0xFF;
    *LAPIC_LVT_TIMER_REGISTER |= INTRCODE_TIMER_INTERRUPT;

    interrupt_set_handler(INTRCODE_TIMER_INTERRUPT, &timer_handler);
}