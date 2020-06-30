#include <fakix/syscall.h>
#include <fakix/capabilities.h>
#include <fakix/tcb.h>
#include <fakix/addr.h>

#define SIZEOF_CAP 32

typedef vaddr_t cap_handle_t;

/* Create either an L1 or L2 cnode. */
errval_t cnode_create(enum cap_object_type cnode_type, cnode_t *ret_cnode, 
                      capaddr_t *ret_cap)
{
    cap_handle_t cap;
    capaddr_t addr;
    void *buffer;
    
    if (cnode_type != CAP_OBJECT_L1 && cnode_type != CAP_OBJECT_L2) {
        return CAP_ERR_INVALID_TYPE_OPERATION;
    }
    
    /* TODO: allocate a 4K page, a capability, a capaddr, then acquire the
       physical mapping and return it into ret_cnode. */

    // *ret_cnode = cap;
    // *ret_cap = addr;

    // return syscall3(SYS_CREATE_CAP, (sysarg_t)buffer, cnode_type, cap);
    return ERR_OK;
}

/* Retype a capability. */
errval_t cap_retype(capaddr_t dest, capaddr_t src, enum cap_object_type objtype,
                    size_t size, ptrdiff_t offset)
{
    return ERR_OK;
}

/* Copy a capability. Intra-CSpace operation. */
errval_t cap_copy(capaddr_t dest, capaddr_t src)
{
    return ERR_OK;
}

/* Empty out a capability slot. */
errval_t cap_destroy(capaddr_t dest)
{
    return ERR_OK;
}

/* Allocate an empty cap or L2 cap. */
errval_t cap_alloc(cnode_t cnode, capaddr_t *ret_cap)
{
    return ERR_OK;
}

errval_t cap_get_info(capaddr_t cap, caprights_t *ret_rights, enum cap_object_type *ret_type)
{
    return ERR_OK;
}

/* Retype a capability. Can be an inter-CSpace or intra-CSpace operation. */
errval_t cap_retype_ref(struct capref dest, struct capref src, 
                        enum cap_object_type objtype, size_t size, 
                        ptrdiff_t offset, caprights_t prop)
{
    return ERR_OK;
}

/* Retype a capability. Can be an inter-CSpace or intra-CSpace operation. */
errval_t cap_copy_ref(struct capref dest, struct capref src, caprights_t prop)
{
    return ERR_OK;
}

errval_t cap_invoke(capaddr_t cap, enum cap_invocation invoke, sysarg_t arg0,
                    sysarg_t arg1, sysarg_t arg2, sysarg_t arg3)
{
    return ERR_OK;
}
