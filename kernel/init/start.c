
#include <stddef.h>
#include <elf/elf.h>
#include <cap/caps.h>
#include <log/print.h>
#include <cap/entry.h>
#include <attributes.h>
#include <sys/vtable.h>
#include <cap/invoke.h>
#include <fakix/vspace.h>
#include <init/startup.h>
#include <sys/task_manager.h>
#include <bootboot/bootboot.h>
#include <fakix/init/initcaps.h>

extern BOOTBOOT bootboot;

typedef struct file {
    uint8_t *ptr;
    uint64_t size;
} file_t;

static file_t tar_initrd(unsigned char *initrd_p, char *kernel);
static struct capability *alloc_init_cap(void);
static void print_mmap_ent(MMapEnt *mm);

static inline errval_t sys_default(void)
{
    return ERR_OK;
}

void *syscall_table[SYS_COUNT] = {
    [0 ... SYS_COUNT - 1] = &sys_default,
    [SYS_INVOKE_CAP] = &caps_invoke,
    [SYS_CREATE_CAP] = &caps_create_entry,
    [SYS_RETYPE_CAP] = &caps_retype,
    [SYS_DESTROY_CAP] = &caps_destroy,
    [SYS_COPY_CAP] = &caps_copy,
    [SYS_MAP_CAP] = &caps_vmap
};

char print_buffer[VSPACE_BASE_PAGE_SIZE];

void start(struct bootinfo *bi)
{
    static char VSPACE_PAGE_ALIGN init_l1cnode_buffer[VSPACE_BASE_PAGE_SIZE];
    static char VSPACE_PAGE_ALIGN init_ramcap_buffer[VSPACE_BASE_PAGE_SIZE];
    static char VSPACE_PAGE_ALIGN init_devframe_buffer[VSPACE_BASE_PAGE_SIZE];
    static char VSPACE_PAGE_ALIGN init_acpi_buffer[VSPACE_BASE_PAGE_SIZE];
    static char VSPACE_PAGE_ALIGN init_task_buffer[VSPACE_BASE_PAGE_SIZE];
    
    struct capability *l1cnode = alloc_init_cap();
    caps_create_l1_cnode(init_l1cnode_buffer, VSPACE_BASE_PAGE_SIZE, l1cnode);

    struct capability *ram_cnode = alloc_init_cap();
    struct capability *dev_cnode = alloc_init_cap();
    struct capability *acpi_cnode = alloc_init_cap();
    struct capability *task_cnode = alloc_init_cap();

    caps_create_l2_cnode(init_ramcap_buffer, VSPACE_BASE_PAGE_SIZE, ram_cnode);
    caps_create_l2_cnode(init_ramcap_buffer, VSPACE_BASE_PAGE_SIZE, dev_cnode);
    caps_create_l2_cnode(init_ramcap_buffer, VSPACE_BASE_PAGE_SIZE, acpi_cnode);
    caps_create_l2_cnode(init_ramcap_buffer, VSPACE_BASE_PAGE_SIZE, task_cnode);

    caps_write_cap(l1cnode, CAP_INIT_RAM_BASE, ram_cnode);
    caps_write_cap(l1cnode, CAP_INIT_DEV_BASE, dev_cnode);
    caps_write_cap(l1cnode, CAP_INIT_ACPI_BASE, acpi_cnode);
    caps_write_cap(l1cnode, CAP_ADDR(CNODE_TASK, 0), task_cnode);

    KERNEL_MSG("initialized init L1/L2 capabilities, onto creating ramcaps");

    capaddr_t ramcap = CAP_INIT_RAM_BASE;
    capaddr_t acpicap = CAP_INIT_ACPI_BASE;
    MMapEnt *mm = &bootboot.mmap;
    uint32_t size = 128;

    while (size != bootboot.size) {
        switch (MMapEnt_Type(mm)) {
            case MMAP_FREE: {
                struct capability *ram_capability = alloc_init_cap();
                ram_capability->base = MMapEnt_Ptr(mm);
                ram_capability->size = MMapEnt_Size(mm);
                ram_capability->objtype = CAP_OBJECT_RAM;
                ram_capability->rights = CAP_RIGHTS_RDWR;
                ramcap++;
                caps_write_cap(ram_cnode, ramcap, ram_capability);
            } break;
        }
        size += sizeof *mm;
        mm++;
    }

    bi->ramcap_count = (ramcap - CAP_INIT_RAM_BASE);
    bi->acpicap_count = (acpicap - CAP_INIT_ACPI_BASE);

    KERNEL_MSG("initialized ramcaps, now creating init task");
    
    static char task_buffer[1 << TASK_BITS];
    struct capability *init_task_cap = alloc_init_cap();
    task_create_from_ramdisk(task_buffer, 1 << TASK_BITS, l1cnode, bootboot.initrd_ptr, "sbin/init", init_task_cap);
    caps_write_cap(l1cnode, CAP_TASK_TASK, init_task_cap);

    task_context_switch((struct task_manager *)init_task_cap->base);
    
    KERNEL_MSG("ERROR!!! Returned from context switch to /sbin/init, spinning");
    while (1) asm("");
}

errval_t task_create_from_ramdisk(void *buf, size_t buflen, struct capability *l1cnode, 
                                  uint8_t *rd, const char *rdname, struct capability *ret_cap)
{
    if (buflen != (1 << TASK_BITS)) {
        return -1;
    }
    
    errval_t err = task_create(buf, buflen, l1cnode, ret_cap);
    if (err_is_fail(err)) {
        return err;
    }

    struct task_manager *tm = buf;
    file_t elf = tar_initrd(rd, (char *)rdname);
    if (elf.ptr == NULL) {
        
    }

    err = task_init((Elf64_Ehdr *)elf.ptr, tm, &alloc_init_cap);
    if (err_is_fail(err)) {
        return err;
    }

    return err;
}

static void print_mmap_ent(MMapEnt *mm)
{
    
    KERNEL_MSG("MMapEnt {\n  %llx\n  %llx\n  %llx\n}", MMapEnt_Type(mm), 
                MMapEnt_Ptr(mm), MMapEnt_Size(mm));
}

static struct capability *alloc_init_cap(void)
{
    static char VSPACE_PAGE_ALIGN init_cap_buffer[VSPACE_BASE_PAGE_SIZE];
    static struct capability *cap_bump_ptr = (struct capability *)init_cap_buffer;
    return (cap_bump_ptr != (struct capability *)(init_cap_buffer + VSPACE_BASE_PAGE_SIZE)) 
            ? cap_bump_ptr++ : NULL;
}

static int memcmp(const void *_fst, const void *_snd, size_t bytes)
{
    const unsigned char *fst = _fst;
    const unsigned char *snd = _snd;
    int dif = 0;

    while (bytes-- > 0) {
        dif = (*fst++ - *snd++);
        if (dif != 0) {
            break;
        }
    }

    return dif;
}

static int oct2bin(uint8_t *str, int size)
{
    int s = 0;
    unsigned char *c = str;
    while (size-- > 0) {
        s *= 8;
        s += *c - '0';
        c++;
    }
    return s;

}

static int strlena(uint8_t *str)
{
    int len = 0;
    while (str[len++] != 0);
    return len;
}

static file_t tar_initrd(unsigned char *initrd_p, char *kernel)
{
    unsigned char *ptr = initrd_p;
    int k;
    file_t ret = { NULL, 0 };
    if (initrd_p == NULL || kernel == NULL || memcmp(initrd_p + 257, "ustar", 5)) {
        return ret;
    }

    k = strlena((unsigned char *)kernel);
    while (!memcmp(ptr + 257, "ustar", 5)) {
        int fs = oct2bin(ptr + 0x7c, 11);
        if (!memcmp(ptr, kernel, k)){
            ret.size = fs;
            ret.ptr = (uint8_t *)(ptr + 512);
            return ret;
        }
        
        ptr += (((fs + 511) / 512) + 1) * 512;
    }
    return ret;
}