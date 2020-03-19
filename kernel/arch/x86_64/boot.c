#include <multiboot/multiboot2.h>
#include <fakix/capabilities.h>
#include <init/startup.h>
#include <fakix/vspace.h>
#include <int/idt_ent.h>
#include <fakix/addr.h>
#include <attributes.h>
#include <cap/caps.h>
#include <elf/elf.h>
#include <stddef.h>
#include <stdint.h>
#include <gdt.h>
#include <msr.h>

#define X86_64_TABLE_OFS(a, l) VSPACE_TABLE_OFFSET(a, l, VSPACE_EXPECTED_PAGE_BITS, VSPACE_EXPECTED_OFFSET_BITS)

static uint8_t init_kernel_stack[VSPACE_BASE_PAGE_SIZE] asm("init_kernel_stack") section(".start.bss");

/* Use this to prevent page faulting after loading in the new page table. */
static size_t bootstrap_pdp_table[VSPACE_TABLE_ENTRIES] section(".start.bss") = {
    [0] = (0x000000083)
};

/* Map the first 8G of the physical address space. */
size_t init_pdp_table[VSPACE_TABLE_ENTRIES] section(".start.data") = {
    [X86_64_TABLE_OFS(VSPACE_KERN_BASE, 3)]     = (0x000000083),
    [X86_64_TABLE_OFS(VSPACE_KERN_BASE, 3) + 1] = (0x080000083),
    [X86_64_TABLE_OFS(VSPACE_KERN_BASE, 3) + 2] = (0x100000083),
    [X86_64_TABLE_OFS(VSPACE_KERN_BASE, 3) + 3] = (0x180000083),
    [X86_64_TABLE_OFS(VSPACE_KERN_BASE, 3) + 4] = (0x200000083),
    [X86_64_TABLE_OFS(VSPACE_KERN_BASE, 3) + 5] = (0x280000083),
    [X86_64_TABLE_OFS(VSPACE_KERN_BASE, 3) + 6] = (0x300000083),
    [X86_64_TABLE_OFS(VSPACE_KERN_BASE, 3) + 7] = (0x380000083)
};

size_t init_pml4_table[VSPACE_TABLE_ENTRIES] asm("init_pml4_table") section(".start.data") = {
    [0] = (((paddr_t)bootstrap_pdp_table - VSPACE_KERN_BASE)),
    [X86_64_TABLE_OFS(VSPACE_KERN_BASE, 4)] = (((paddr_t)init_pdp_table - VSPACE_KERN_BASE))
};

static struct gdt gdt section(".start.data") = {
    .descriptors = {
        [0] = GDT_KERNEL_CODE_64,
        [1] = GDT_KERNEL_DATA_64,
        [2] = GDT_USER_CODE_64,
        [3] = GDT_USER_DATA_64
    }
};

static struct gdt_pointer gdtp asm("gdtp") section(".start.data") = {
    .size = sizeof gdt,
    .gdt_addr = (((paddr_t)&gdt) - VSPACE_KERN_BASE)
};

section(".start") void _start(void)
{
    asm volatile (
        ".code32\n\t"
        "movl %eax, %edi\n\t"
        /* Disable paging for now. */
        "movl %cr0, %eax\n\t"
        "andl $(~(1 << 31)), %eax\n\t"
        "movl %eax, %cr0\n\t"
        /* Set the present and writable bits in the 256th and 0th entries in
           the PML4 table, as a quirk in designated array initialization rules
           that these operations cannot be constant. */
        "orl $0x03, (init_pml4_table)\n\t"
        "orl $0x03, (init_pml4_table + 256)\n\t"
        "movl $(init_pml4_table), %eax\n\t"
        "movl %eax, %cr3\n\t"
        /* Set the PAE bit, which allows for 48-bit virtual addresses. */
        "movl %cr4, %eax\n\t"
        "orl $(1 << 5), %eax\n\t"
        "movl %eax, %cr4\n\t"
        /* Enable long mode in the IA32_EFER msr. */
        "movl $0xC0000080, %ecx\n\t"
        "rdmsr\n\t"
        "orl $(1 << 8), %eax\n\t"
        "wrmsr\n\t"
        /* Reenable paging using our new upper-half kernel page table. */
        "movl %cr0, %eax\n\t"
        "orl $(1 << 31), %eax\n\t"
        "movl %eax, %cr0\n\t"
        /* Setup the new upper-half kernel stack. */
        "movl $init_kernel_stack, %esp\n\t"
        /* Load the GDT. */
        "lgdt (gdtp)\n\t"
        /* This does two very important things: Firstly, it finalizes the jump
           into long mode; secondly, it sets the PC to conform with the upper 
           half kernel model. */
        "ljmp $0x08, $1f\n\t"
        ".code64\n\t"
        "1:\n\t"
        "movw $0, %ax\n\t"
        "movw %ax, %ds\n\t"
        "movw %ax, %es\n\t"
        "movw %ax, %fs\n\t"
        "movw %ax, %gs\n\t"
        "movw %ax, %ss\n\t"
        "movl %edi, %eax\n\t"
        "jmp start"
    );
}