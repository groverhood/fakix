#ifndef FAKIX_LIBFAK_TASK_H
#define FAKIX_LIBFAK_TASK_H 1

#include <sys.h>
#include <cbl.h>
#include <cmd.h>
#include <stdint.h>
#include <util/list.h>

/* Segment selector type. This data type is unused in ARM. */
typedef uint16_t segsel_t;

/* Instruction pointer type. */
typedef uintptr_t progcnt_t;

typedef uint8_t taskpriority_t;

typedef int pid_t;

/* Task control block data structure. */
struct task {
	pid_t pid; /* Process identifier. */
	segsel_t cs; /* Code segment selector. Ignored on ARM. */
	segsel_t ss; /* Stack segment selector. Also ignored on ARM. */
	progcnt_t pc; /* Program counter. */
	taskpriority_t priority;
	uintptr_t sp; /* Stack pointer. */
	uintptr_t *page_table; /* Top-level page table. */
	struct cnode l2node; /* L2 CNode. */
	struct list_elem sched_elem;
};

void task_yield(struct task *tcb);
void task_block(struct task *tcb);
void task_unblock(struct task *tcb);

#endif