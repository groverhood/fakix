#include <fakix/capabilities.h>


/* Create either an L1 or L2 cnode. */
errval_t cnode_create(enum cap_object_type cnode_type, cnode_t *ret_cnode, 
                      capaddr_t *ret_cap)
{
    
}

/* Retype a capability. */
errval_t cap_retype(capaddr_t dest, capaddr_t src, enum cap_object_type objtype,
                    size_t size, ptrdiff_t offset)
{

}

/* Copy a capability. Intra-CSpace operation. */
errval_t cap_copy(capaddr_t dest, capaddr_t src)
{

}

/* Empty out a capability slot. */
errval_t cap_destroy(capaddr_t dest)
{

}

/* Allocate an empty cap or L2 cap. */
errval_t cap_alloc(cnode_t cnode, capaddr_t *ret_cap)
{

}

errval_t cap_get_info(capaddr_t cap, caprights_t *ret_rights, enum cap_object_type *ret_type)
{

}

/* Retype a capability. Can be an inter-CSpace or intra-CSpace operation. */
errval_t cap_retype_ref(struct capref dest, struct capref src, 
                        enum cap_object_type objtype, size_t size, 
                        ptrdiff_t offset, caprights_t prop)
{

}

/* Retype a capability. Can be an inter-CSpace or intra-CSpace operation. */
errval_t cap_copy_ref(struct capref dest, struct capref src, caprights_t prop)
{

}

errval_t cap_invoke(capaddr_t cap, enum cap_invocation invoke, sysarg_t arg0,
                    sysarg_t arg1, sysarg_t arg2, sysarg_t arg3)
{

}