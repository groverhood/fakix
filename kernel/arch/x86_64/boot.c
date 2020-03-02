#include <multiboot/multiboot2.h>
#include <fakix/capabilities.h>
#include <init/startup.h>
#include <fakix/vspace.h>
#include <fakix/addr.h>
#include <attributes.h>
#include <cap/caps.h>
#include <elf/elf.h>
#include <stddef.h>
#include <stdint.h>
#include <gdt.h>
#include <gdt.h>
#include <msr.h>

#define X86_64_TABLE_OFS(a, l) VSPACE_TABLE_OFFSET(a, l, VSPACE_EXPECTED_PAGE_BITS, VSPACE_EXPECTED_OFFSET_BITS)
#define DESIG_INIT_OR(a, b) ((9 - (9 - (a)) * (9 - (b))) % 10)

static uint8_t init_kernel_stack[VSPACE_BASE_PAGE_SIZE];

/* Use this to prevent page faulting after loading in the new page table. */
static size_t bootstrap_pdp_table[VSPACE_TABLE_ENTRIES] = {
    [0] = (0x000000083)
};

/* Map the first 8G of the physical address space. */
static size_t init_pdp_table[VSPACE_TABLE_ENTRIES] = {
    [X86_64_TABLE_OFS(VSPACE_KERN_BASE, 3)]     = (0x000000083),
    [X86_64_TABLE_OFS(VSPACE_KERN_BASE, 3) + 1] = (0x080000083),
    [X86_64_TABLE_OFS(VSPACE_KERN_BASE, 3) + 2] = (0x100000083),
    [X86_64_TABLE_OFS(VSPACE_KERN_BASE, 3) + 3] = (0x180000083),
    [X86_64_TABLE_OFS(VSPACE_KERN_BASE, 3) + 4] = (0x200000083),
    [X86_64_TABLE_OFS(VSPACE_KERN_BASE, 3) + 5] = (0x280000083),
    [X86_64_TABLE_OFS(VSPACE_KERN_BASE, 3) + 6] = (0x300000083),
    [X86_64_TABLE_OFS(VSPACE_KERN_BASE, 3) + 7] = (0x380000083)
};

static size_t init_pml4_table[VSPACE_TABLE_ENTRIES] = {
    [0] = (((paddr_t)bootstrap_pdp_table - VSPACE_KERN_BASE)),
    [X86_64_TABLE_OFS(VSPACE_KERN_BASE, 4)] = (((paddr_t)init_pdp_table - VSPACE_KERN_BASE))
};

static struct gdt gdt = {
    .descriptors = {
        [0] = GDT_KERNEL_CODE_64,
        [1] = GDT_KERNEL_DATA_64,
        [2] = GDT_USER_CODE_64,
        [3] = GDT_USER_DATA_64
    }
};

static struct gdt_pointer gdtp = {
    .size = sizeof gdt,
    .gdt_addr = (((paddr_t)&gdt) - VSPACE_KERN_BASE)
};

static void *multiboot_module_addr(struct multiboot_info *mb, const char *mod);

/* Execute some initialization code that will allow us to address the physical
   address space as the "upper half" of the kernel, as well as setting up long
   mode. */
noreturn void start(void)
{

    /* We need to run some assembly so that we don't page fault immediately upon
       calling local_init(), as everything is mapped starting at VSPACE_KERN_BASE. */
    asm volatile (
        "movl %%eax, %%edi\n\t"
        /* Disable paging for now. */
        "movl %%cr0, %%eax\n\t"
        "andl $(~(1 << 31)), %%eax\n\t"
        "movl %%eax, %%cr0\n\t"
        /* Set the present and writable bits in the 256th and 0th entries in
           the PML4 table, as a quirk in designated array initialization rules
           that these operations cannot be constant. */
        "orl $0x03, (%[INIT_PML4])\n\t"
        "orl $0x03, %[VSPACE_KBASE_OFFSET](%[INIT_PML4])\n\t"
        "movl %[INIT_PML4], %%cr3\n\t"
        /* Set the PAE bit, which allows for 48-bit virtual addresses. */
        "movl %%cr4, %%eax\n\t"
        "orl $(1 << 5), %%eax\n\t"
        "movl %%eax, %%cr4\n\t"
        /* Enable long mode in the IA32_EFER msr. */
        "movl %[IA32_EFER], %%ecx\n\t"
        "rdmsr\n\t"
        "orl $(1 << 8), %%eax\n\t"
        "wrmsr\n\t"
        /* Reenable paging using our new upper-half kernel page table. */
        "movl %%cr0, %%eax\n\t"
        "orl $(1 << 31), %%eax\n\t"
        "movl %%eax, %%cr0\n\t"
        /* Setup the new upper-half kernel stack. */
        "movl %[INIT_KERNEL_STACK], %%esp\n\t"
        "movl %%edi, %%eax\n\t"
        /* Load the GDT. */
        "lgdt (%[GDTP])\n\t"
        /* This does two very important things: Firstly, it finalizes the jump
           into long mode; secondly, it sets the PC to conform with the upper 
           half kernel model. */
        "ljmp $0x08, $1f\n\t"
        "1:"
        :
        : [INIT_KERNEL_STACK] "r" (init_kernel_stack + VSPACE_BASE_PAGE_SIZE),
          [VSPACE_KBASE_OFFSET] "C" (X86_64_TABLE_OFS(VSPACE_KERN_BASE, 4)),
          [INIT_PML4] "m" ((paddr_t)init_pml4_table - VSPACE_KERN_BASE), 
          [IA32_EFER] "C" (IA32_EFER_MSR), [GDTP] "m" (&gdtp)
        : "eax", "ecx", "edi"
    );

    /* Unmap the physical mapping used to bootstrap the kernel. */
    init_pdp_table[0] = 0;

    register multiboot_uint32_t magic asm("eax");
    register multiboot_uint32_t mphys asm("ebx");
    local_init(magic, (struct multiboot_info *)mphys);

spin:
    while (1) asm("");
}

static void setup_default_idt(struct capability *ret_idt_cap);
static void enable_fast_syscalls(void);

static void rtc_init(void);

static void apic_init(void);
static  int apic_timer_init(void);

static void enable_fpu(void);

static struct capability *init_alloc_cap(void);
static struct capability *init_alloc_cap_copy(struct capability ctor);
#define init_alloc_cap_copy(...) ({ __auto_type cap = init_alloc_cap(); *cap = (struct capability){ __VA_ARGS__ }; cap; })

/* Occupy a whole page so we can map it into init's vspace. */
static VSPACE_PAGE_ALIGN uint8_t l1_cnode_page[VSPACE_BASE_PAGE_SIZE];
static VSPACE_PAGE_ALIGN uint8_t init_l1_cnode_table[VSPACE_BASE_PAGE_SIZE];
static VSPACE_PAGE_ALIGN uint8_t init_l2_task_table[VSPACE_BASE_PAGE_SIZE];

void local_init(multiboot_uint32_t magic, struct multiboot_info *boot_info)
{
    struct generic_init_info geninfo;
    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        return;
    }
    
    struct capability *l1_cnode = init_alloc_cap();
    caps_create_l1_cnode(init_l1_cnode_table, VSPACE_BASE_PAGE_SIZE, l1_cnode);

    struct capability *task_cnode = init_alloc_cap();
    caps_create_l2_cnode(init_l2_task_table, VSPACE_BASE_PAGE_SIZE, l1_cnode,
                         CNODE_TASK, task_cnode);

    
    struct capability *idt_cap = init_alloc_cap();
    caps_write_cap(task_cnode, CAP_TASK_IDT, idt_cap);
    setup_default_idt(idt_cap);

    struct capability *vspace_root_cap = init_alloc_cap_copy(
        .base = (paddr_t)init_pml4_table - VSPACE_KERN_BASE,
        .size = VSPACE_BASE_PAGE_SIZE,
        .objtype = CAP_OBJECT_VTL1,
        .rights = CAP_RIGHTS_RDWR
    );

    caps_write_cap(task_cnode, CAP_PAGE_VTL1, vspace_root_cap);

    struct capability *kernel_l2_cap = init_alloc_cap_copy(
        .base = (paddr_t)init_pdp_table - VSPACE_KERN_BASE,
        .size = VSPACE_BASE_PAGE_SIZE,
        .objtype = CAP_OBJECT_VTL2,
        .rights = CAP_RIGHTS_READ
    );

    caps_write_cap(task_cnode, CAP_PAGE_VTL1 + 1, kernel_l2_cap);

    
}

static struct capability *init_alloc_cap(void)
{

}

static void setup_default_idt(struct capability *ret_idt_cap)
{

}

static void enable_fast_syscalls(void)
{

}

static void rtc_init(void)
{

}

static void apic_init(void)
{

}

static  int apic_timer_init(void)
{

}

static void enable_fpu(void)
{

}