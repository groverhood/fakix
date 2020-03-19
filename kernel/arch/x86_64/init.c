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

extern size_t init_pdp_table[VSPACE_TABLE_ENTRIES];
extern size_t init_pml4_table[VSPACE_TABLE_ENTRIES];

static void *multiboot_module_addr(struct multiboot_info *mb, const char *mod);
/* Execute some initialization code that will allow us to address the physical
   address space as the "upper half" of the kernel, as well as setting up long
   mode. */
noreturn void start(void)
{
    /* Unmap the physical mapping used to bootstrap the kernel. */
    init_pdp_table[0] = 0;

    register multiboot_uint32_t magic asm("eax");
    register multiboot_uint64_t mphys asm("rbx");
    local_init(magic, (struct multiboot_info *)(mphys + VSPACE_KERN_BASE));

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

    while (1) asm("");
}

static struct capability *init_alloc_cap(void)
{
    static struct capability *cap = (struct capability *)l1_cnode_page;
    return (cap != (struct capability *)l1_cnode_page + VSPACE_BASE_PAGE_SIZE) ? cap++ : NULL;
}

static void setup_default_idt(struct capability *ret_idt_cap)
{
    idt_init(ret_idt_cap);
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