/**
 *  === syscall.c ===
 *  
 *  Implement stubs for every system call.
 **/

#include <sys.h>

/* === SYS.H === */

bool sys_info(sysnum_t sysn, struct syscall_info *res_info)
{
	return sys_trap(SYSNUM_SYS_INFO, bool, sysn, res_info);
}

/* === CMD.H === */

#include <cmd.h>

cmdstat_t cmd_send(pid_t proc, const struct command *cmd)
{
	return sys_trap(SYS_CMD_SEND, cmdstat_t, proc, cmd);
}

cmdstat_t cmd_recv(struct command *cmd)
{
	return sys_trap(SYS_CMD_RECV, cmdstat_t, cmd);
}

/* === TASK.H === */

#include <task.h>

void task_sched_next(struct task *prev, struct task *next)
{
	sys_trap(SYS_TASK_SCHED_NEXT, int, prev, next);
}