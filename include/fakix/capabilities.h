#ifndef FAKIX_CAPABILITIES_H
#define FAKIX_CAPABILITIES_H 1

#include <stddef.h>
#include <fakix/vspace.h>
#include <fakix/cspace.h>
#include <fakix/errtype.h>
#include <fakix/syscall.h>

#define CTE_SIZE (64)
#define CTE_COUNT (PAGE_SIZE / WORD_SIZE)

/* Object type. */
enum cap_object_type {
    CAP_OBJECT_NULL,
    CAP_OBJECT_L1, /* L1 cnode table. */
    CAP_OBJECT_L2, /* L2 cnode table. */
    CAP_OBJECT_PHYSICAL, /* Physical address region. */
    CAP_OBJECT_TCB, /* Task control block. */
    CAP_OBJECT_RAM, /* Raw, untyped RAM. */
    CAP_OBJECT_FRAME, /* Frame to be mapped into virtual memory. */
    CAP_OBJECT_DEVICE, /* MMIO region. */
    CAP_OBJECT_IPC, /* IPC data block. */
    CAP_OBJECT_IDT, /* Interrupt descriptor table. */

    CAP_OBJECT_VTL1, /* Level 1 VSpace table. */
    CAP_OBJECT_VTL2, /* Level 2 VSpace table. */
    CAP_OBJECT_VTL3, /* Level 3 VSpace table. */
    CAP_OBJECT_VTL4  /* Level 4 VSpace table. */
};

typedef uint32_t caprights_t;

#define CAP_RIGHTS_READ (0 << 1)
#define CAP_RIGHTS_RDWR (1 << 1)

/**
 *  All capability creation in the user space descends from acquired RAM caps.
 * 
 *  The general flow goes like this:
 *    1. The spawnd process is created and handed RAM capabilities made by the
 *       kernel
 *    2. An arbitary process dispatches a message to spawnd requesting ram caps
 *    3. The spawnd process satisfies the request
 *    4. The requesting process retypes the ram caps into what it needs
 **/

/* Create either an L1 or L2 cnode. */
errval_t cnode_create(enum cap_object_type cnode_type, cnode_t *ret_cnode, 
                      capaddr_t *ret_cap);

/* Retype a capability. */
errval_t cap_retype(capaddr_t dest, capaddr_t src, enum cap_object_type objtype,
                    size_t size, ptrdiff_t offset);

/* Copy a capability. Intra-CSpace operation. */
errval_t cap_copy(capaddr_t dest, capaddr_t src);

/* Empty out a capability slot. */
errval_t cap_destroy(capaddr_t dest);

/* Allocate an empty cap or L2 cap. */
errval_t cap_alloc(cnode_t cnode, capaddr_t *ret_cap);

errval_t cap_get_info(capaddr_t cap, caprights_t *ret_rights, enum cap_object_type *ret_type);

/* Globally visible capability. */
struct capref {
    cnode_t l1_cnode;
    capaddr_t cap;
};

#define CAPREF_NULL ((struct capref){ .l1_cnode = CNODE_NULL, .cap = CAP_NULL })

/* Retype a capability. Can be an inter-CSpace or intra-CSpace operation. */
errval_t cap_retype_ref(struct capref dest, struct capref src, 
                        enum cap_object_type objtype, size_t size, 
                        ptrdiff_t offset, caprights_t prop);

/* Retype a capability. Can be an inter-CSpace or intra-CSpace operation. */
errval_t cap_copy_ref(struct capref dest, struct capref src, caprights_t prop);

enum cap_invocation {
    INVOKE_TASK,
};

errval_t cap_invoke(capaddr_t cap, enum cap_invocation invoke, sysarg_t arg0,
                    sysarg_t arg1, sysarg_t arg2, sysarg_t arg3);

#endif