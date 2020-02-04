
#include <io/dev.h>
#include <io/disk.h>
#include <io/disk/ahci.h>

static disk_read_op_t *disk_read_op;
static disk_write_op_t *disk_write_op;

#define DEV_AHCI 0x01

void init_disk_driver(void)
{
    struct fakix_device *dev;

    if ((dev = get_device(DEV_STORAGE, DEV_AHCI, -1)) != NULL) {
        init_ahci_driver(dev);
        disk_read_op = &ahci_disk_read;
        disk_write_op = &ahci_disk_write;
    }
}

size_t disk_read(void *dest, lbaddr_t src, size_t bytes)
{
    return disk_read_op(dest, src, bytes);
}

size_t disk_write(lbaddr_t dest, const void *src, size_t bytes)
{
    return disk_write_op(dest, src, bytes);
}
