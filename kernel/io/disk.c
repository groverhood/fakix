#include <io/disk.h>
#include <io/dev.h>
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

ssize_t disk_read(void *dest, off_t start, size_t bytes)
{
	return disk_read_op(dest, start, bytes);
}

ssize_t disk_write(const void *src, off_t start, size_t bytes)
{
	return disk_write_op(src, start, bytes);
}