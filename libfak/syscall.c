/**
 *  === syscall.c ===
 *  
 *  Implement stubs for every system call.
 **/

#include <sys.h>

#define _foo(x) #x
#define foo(x) _foo(x)

bool sys_info(sysnum_t sysn, struct syscall_info *res_info)
{
	return sys_trap(SYSNUM_SYS_INFO, bool, sysn, res_info);
}