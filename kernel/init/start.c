
#include <stddef.h>
#include <elf/elf.h>
#include <cap/caps.h>
#include <attributes.h>
#include <sys/vtable.h>
#include <fakix/vspace.h>
#include <init/startup.h>
#include <bootboot/bootboot.h>
#include <fakix/init/initcaps.h>

extern BOOTBOOT bootboot;

typedef struct file {
    uint8_t *ptr;
    uint64_t size;
} file_t;

static file_t tar_initrd(unsigned char *initrd_p, char *kernel);
static struct capability *alloc_init_cap(void);

void start(struct bootinfo *bi)
{
    static char VSPACE_PAGE_ALIGN init_l1cnode_buffer[VSPACE_BASE_PAGE_SIZE];
    
    struct capability *l1cnode = alloc_init_cap();
    caps_create_l1_cnode(init_l1cnode_buffer, VSPACE_BASE_PAGE_SIZE, l1cnode);

    struct capability *ram_cnode = alloc_init_cap();
    struct capability *dev_cnode = alloc_init_cap();
    struct capability *acpi_cnode = alloc_init_cap();

    caps_write_cap(l1cnode, CAP_INIT_RAM_BASE, ram_cnode);
    caps_write_cap(l1cnode, CAP_INIT_DEV_BASE, dev_cnode);
    caps_write_cap(l1cnode, CAP_INIT_ACPI_BASE, acpi_cnode);
    
    capaddr_t ramcap = CAP_INIT_RAM_BASE;
    capaddr_t devcap = CAP_INIT_DEV_BASE;
    capaddr_t acpicap = CAP_INIT_ACPI_BASE;
    MMapEnt *mm = &bootboot.mmap;

    while (MMapEnt_Ptr(mm) != 0) {
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
        mm++;
    }

    file_t init = tar_initrd((uint8_t *)bootboot.initrd_ptr, "sbin/init");
    Elf64_Ehdr *ehdr = (Elf64_Ehdr *)init.ptr;
    
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
    unsigned char *fst = _fst;
    unsigned char *snd = _snd;
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
        if (!memcmp(ptr, kernel, k + 1)){
            ret.size = fs;
            ret.ptr = (uint8_t *)(ptr + 512);
            return ret;
        }
        
        ptr += (((fs + 511) / 512) + 1) * 512;
    }
    return ret;
}