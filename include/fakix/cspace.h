#ifndef FAKIX_CSPACE_H
#define FAKIX_CSPACE_H 1

#include <stdint.h>
#include <fakix/addr.h>

/* Address in process's CSpace. */
typedef uint32_t capaddr_t;

/* Index into a CNode. */
typedef size_t capslot_t;

/* CNode handle. */
typedef vaddr_t cnode_t;

#define CNODE_NULL ((cnode_t)0)

#define CAP_L1_BITS 9
#define CAP_L2_BITS 9

#define CAP_L2_OFFSET_MASK PAGE_OFFSET_MASK(CAP_L2_BITS)
#define CAP_L2_OFFSET(caddr) ((caddr) & CAP_L2_OFFSET_MASK)

#define CAP_L1_OFFSET_MASK PAGE_OFFSET_MASK(CAP_L1_BITS)
#define CAP_L1_OFFSET(caddr) (((caddr) >> CAP_L2_BITS) & CAP_L1_OFFSET_MASK) 

#define CAP_ADDR(cnode, slot) ((capaddr_t)(((cnode) << CAP_L2_BITS) | (slot)))

/* Well-known capabilities and cnodes. */

extern cnode_t cnode_task;
extern cnode_t cnode_slot;

extern capaddr_t cap_vspace_root; /* Root page table. PML4 in x86_64, ARMl1, etc. */
extern capaddr_t cap_tcb; /* Task control block. */
extern capaddr_t cap_init_pages; /* Pages used to bootstrap a process. */
extern capaddr_t cap_args_page; /* Page containing argv[]. */
extern capaddr_t cap_task;

extern capaddr_t cap_cspace_root; /* L1 CNode capability. */
extern capaddr_t cap_slot_alloc_0;
extern capaddr_t cap_slot_alloc_1;
extern capaddr_t cap_slot_alloc_2;

#define CAP_NULL ((capaddr_t)0)

#define CNODE_TASK 0
#define CNODE_TASK_SLOT_VSPACE_ROOT 1
#define CNODE_TASK_SLOT_TCB 2
#define CNODE_TASK_SLOT_INIT_PAGES 3
#define CNODE_TASK_SLOT_ARGS_PAGE 4
#define CNODE_TASK_SLOT_CSPACE_ROOT 5
#define CNODE_TASK_SLOT_TASK 6
#define CNODE_TASK_SLOT_PAGING_STATE 7
/* Usually unavailable. */
#define CNODE_TASK_SLOT_IDT 8

#define CAP_TASK_VSPACE_ROOT CAP_ADDR(CNODE_TASK, CNODE_TASK_SLOT_VSPACE_ROOT)
#define CAP_TASK_TCB CAP_ADDR(CNODE_TASK, CNODE_TASK_SLOT_TCB)
#define CAP_TASK_INIT_PAGES CAP_ADDR(CNODE_TASK, CNODE_TASK_SLOT_INIT_PAGES)
#define CAP_TASK_ARGS_PAGE CAP_ADDR(CNODE_TASK, CNODE_TASK_SLOT_ARGS_PAGE)
#define CAP_TASK_CSPACE_ROOT CAP_ADDR(CNODE_TASK, CNODE_TASK_SLOT_CSPACE_ROOT)
#define CAP_TASK_TASK CAP_ADDR(CNODE_TASK, CNODE_TASK_SLOT_TASK)
#define CAP_TASK_PAGING_STATE CAP_ADDR(CNODE_TASK, CNODE_TASK_SLOT_PAGING_STATE)
/* Usually unavailable. */
#define CAP_TASK_IDT CAP_ADDR(CNODE_TASK, CNODE_TASK_SLOT_IDT)

#define CNODE_SLOT_ALLOC_0 1
#define CNODE_SLOT_ALLOC_1 2
#define CNODE_SLOT_ALLOC_2 3

#define CNODE_PAGE 4
#define CNODE_PAGE_SLOT_VTL1 0

#define CAP_PAGE_VTL1 CAP_ADDR(CNODE_PAGE, CNODE_PAGE_SLOT_VTL1)

#endif