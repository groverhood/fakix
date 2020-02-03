#include <syscall.h>

void *syscall_table[SYSNUM_MAX + 1];

void syscall_map(sysnum_t sysn, const struct syscall_info *info)
{
	if (sysn >= 0 && sysn <= SYSNUM_MAX) {

	}
}

void syscall_add(const struct syscall_info *info)
{
	sysnum_t sysn;
	for (sysn = 0; sysn <= SYSNUM_MAX; ++sysn) {

	}
}