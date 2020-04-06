#ifndef KERNEL_CAP_CAPS_H
#define KERNEL_CAP_CAPS_H 1

#include <stddef.h>
#include <stdint.h>
#include <fakix/capabilities.h>
#include <fakix/tcb.h>

struct capability {
    enum cap_object_type objtype;
    caprights_t rights;
    paddr_t base;
    size_t size;
};

errval_t caps_create_l1_cnode(void *table, size_t buflen, struct capability *ret_cap);

errval_t caps_create_l2_cnode(void *table, size_t buflen, struct capability *ret_cap);

errval_t caps_create(void *table, size_t buflen, enum cap_object_type objtype,
                     caprights_t rights, struct capability *ret_cap);

/* Entry point for SYS_CREATE_CAP. */
errval_t caps_create_entry(void *table, size_t buflen, enum cap_object_type objtype,
                           caprights_t rights, struct capability *ret_cap);

errval_t caps_retype(struct capability *dest, struct capability *src, 
                     enum cap_object_type objtype, size_t size, ptrdiff_t offset);

errval_t caps_copy(struct capability *dest, struct capability *src);

errval_t caps_destroy(struct capability *dest);

errval_t caps_lookup_cap(struct capability *cnode, capaddr_t caddr, struct capability **ret_cap);

errval_t caps_write_cap(struct capability *cnode, capaddr_t caddr, struct capability *cap);

static struct capability *caps_get_cnode(cnode_t cnode)
{
    return (struct capability *)cnode;
}

errval_t caps_create_vtable(void *table, size_t buflen, enum cap_object_type objtype, bool readonly, struct capability *ret_cap);

errval_t caps_vmap(struct capability *dest, struct capability *src, ptrdiff_t off, capslot_t slot, capslot_t nslots, int flags);

#endif