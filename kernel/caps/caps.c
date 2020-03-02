#include <cap/caps.h>
#include <util/arith.h>

errval_t caps_create_l1_cnode(void *table, size_t buflen, struct capability *ret_cap)
{
    struct capability **l2_caps = table;
    size_t i;
    for (i = 0; i < VSPACE_BASE_PAGE_SIZE / sizeof *l2_caps; ++i) {
        l2_caps[i] = NULL;
    }

    ret_cap->base = (paddr_t)table - VSPACE_KERN_BASE;
    ret_cap->objtype = CAP_OBJECT_L1;
    ret_cap->size = buflen;
    
    return 0;
}

static errval_t cap_retype_physical(struct capability *dest, struct capability *src, 
                     enum cap_object_type objtype, size_t size, ptrdiff_t offset)
{
    dest->base = src->base + offset;
    dest->size = size;
    dest->objtype = objtype;
}

static errval_t cap_retype_ram(struct capability *dest, struct capability *src, 
                     enum cap_object_type objtype, size_t size, ptrdiff_t offset)
{
    errval_t err = 0;
    if (objtype == CAP_OBJECT_DEVICE) {
        err = 1;
    } else {
        dest->base = src->base + offset;
        dest->size = size;
        dest->objtype = objtype;
    }

    return err;
}

static errval_t cap_retype_frame(struct capability *dest, struct capability *src, 
                     enum cap_object_type objtype, size_t size, ptrdiff_t offset)
{
    errval_t err = 0;
    switch (objtype) {
        case CAP_OBJECT_DEVICE:
        case CAP_OBJECT_RAM: {
            err = 1;
        } break;
        default: {
            dest->base = src->base + offset;
            dest->size = size;
            dest->objtype = objtype;
        }
    }
    return err;
}

errval_t caps_retype(struct capability *dest, struct capability *src, 
                     enum cap_object_type objtype, size_t size, ptrdiff_t offset)
{
    errval_t err = 0;
    if (offset < 0) {
        err = 1;
    } else if (size > src->size) {
        err = 1;
    } else {
        size = divide_round_up(size, VSPACE_BASE_PAGE_SIZE);
        offset = divide_round_up(offset, VSPACE_BASE_PAGE_SIZE);
        switch (src->objtype) {
            case CAP_OBJECT_PHYSICAL: {
                err = cap_retype_physical(dest, src, objtype, size, offset);
            } break;
            case CAP_OBJECT_RAM: {
                err = cap_retype_ram(dest, src, objtype, size, offset); 
            } break;
            case CAP_OBJECT_FRAME: { 
                err = cap_retype_frame(dest, src, objtype, size, offset); 
            } break;
            default: {
                err = 1;
            }
        }
    }

    return err;
}

errval_t caps_copy(struct capability *dest, struct capability *src)
{
    dest->base = src->base;
    dest->size = src->size;
    dest->objtype = src->objtype;
    return 0;
}

errval_t caps_destroy(struct capability *dest)
{
    dest->base = 0;
    dest->size = 0;
    dest->objtype = CAP_OBJECT_NULL;
    return 0;
}

errval_t caps_lookup_cap(struct capability *cnode, capaddr_t caddr, struct capability **ret_cap)
{
    errval_t err = 0;
    struct capability **caps = (struct capability **)(cnode->base + VSPACE_KERN_BASE);
    if (cnode->objtype == CAP_OBJECT_L1) {
        *ret_cap = caps[CAP_L1_OFFSET(caddr)];
    } else if (cnode->objtype == CAP_OBJECT_L2) {
        *ret_cap = caps[CAP_L2_OFFSET(caddr)];
    } else {
        err = 1;
    }
    return err;
}

errval_t caps_write_cap(struct capability *cnode, capaddr_t caddr, struct capability *cap)
{
    errval_t err = 0;
    struct capability **caps = (struct capability **)(cnode->base + VSPACE_KERN_BASE);
    if (cnode->objtype == CAP_OBJECT_L1) {
        caps[CAP_L1_OFFSET(caddr)] = cap;
    } else if (cnode->objtype == CAP_OBJECT_L2) {
        caps[CAP_L2_OFFSET(caddr)] = cap;
    } else {
        err = 1;
    }
    return err;
}