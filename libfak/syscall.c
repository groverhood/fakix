/**
 *  === syscall.c ===
 *  
 *  Implement stubs for every system call.
 **/

#include <sys.h>

/* === SYS.H === */

/* Acquire system call info. */
#define SYSNUM_SYS_INFO SYSNUM(0x00)

bool sys_info(sysnum_t sysn, struct syscall_info *res_info)
{
	return sys_trap(SYSNUM_SYS_INFO, bool, sysn, res_info);
}

/* === CMD.H === */

#include <cmd.h>

/* Send command. */
#define SYS_CMD_SEND SYSNUM(0x80)
/* Receive command. */
#define SYS_CMD_RECV SYSNUM(0x81)

cmdstat_t cmd_send(pid_t proc, const struct command *cmd)
{
	return sys_trap(SYS_CMD_SEND, cmdstat_t, proc, cmd);
}

cmdstat_t cmd_recv(struct command *cmd)
{
	return sys_trap(SYS_CMD_RECV, cmdstat_t, cmd);
}