#ifndef FAKIX_LIBFAK_TASK_H
#define FAKIX_LIBFAK_TASK_H 1

#include <sys.h>
#include <stdint.h>
#include <sys/types.h>

/* Schedule next process. */
#define SYS_TASK_SCHED_NEXT SYSNUM(0x10)

/* Segment selector type. This data type is unused in ARM. */
typedef uint16_t segsel_t;

/* Instruction pointer type. */
typedef uintptr_t progcnt_t;

/* Task control block data structure. */
struct task {
	pid_t pid; /* Process identifier. */
	segsel_t cs; /* Code segment selector. Ignored on ARM. */
	segsel_t ss; /* Stack segment selector. Also ignored on ARM. */
	progcnt_t pc; /* Program counter. */
	uint64_t sp; /* Stack pointer. */
};

void task_switch_to(struct task *prev, struct task *next);

#endif