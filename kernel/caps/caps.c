#include <cap/caps.h>
#include <util/arith.h>
#include <sys/vtable.h>
#include <fakix/errtype.h>

errval_t caps_create_l1_cnode(void *table, size_t buflen, struct capability *ret_cap)
{
    paddr_t addr;
    errval_t err = vtable_get_mapping(vtable_current(), (vaddr_t)table, &addr);
    if (err_is_fail(err)) {
        return err;
    }

    size_t i;
    struct capability **l2_caps = table;
    for (i = 0; i < VSPACE_BASE_PAGE_SIZE / sizeof *l2_caps; ++i) {
        l2_caps[i] = NULL;
    }

    ret_cap->base = addr;
    ret_cap->objtype = CAP_OBJECT_L1;
    ret_cap->size = buflen;
    ret_cap->rights = CAP_RIGHTS_RDWR;
    
    return ERR_OK;
}

errval_t caps_create_l2_cnode(void *table, size_t buflen, struct capability *l1_cnode_cap,
                              capslot_t slot, struct capability *ret_cap)
{
    errval_t err = ERR_OK;
    struct capability **l2_caps = (struct capability **)(l1_cnode_cap->base + VSPACE_KERN_BASE);
    if (l2_caps[slot] != NULL) {
        err = CAP_ERR_WRITE_ALLOCATED_CAP;
    } else if (buflen != VSPACE_BASE_PAGE_SIZE) {

    } else {
        paddr_t addr;
        err = vtable_get_mapping(vtable_current(), (vaddr_t)table, &addr);
        if (err_is_fail(err)) {
            return err;
        }

        size_t i;
        struct capability **caps = table;
        for (i = 0; i < VSPACE_BASE_PAGE_SIZE / sizeof *caps; ++i) {
            caps[i] = NULL;
        }

        ret_cap->base = addr;
        ret_cap->objtype = CAP_OBJECT_L2;
        ret_cap->size = buflen;
        ret_cap->rights = CAP_RIGHTS_RDWR;
        l2_caps[slot] = ret_cap;
    }

    return err;
}

static errval_t cap_retype_physical(struct capability *dest, struct capability *src, 
                     enum cap_object_type objtype, size_t size, ptrdiff_t offset)
{
    dest->base = src->base + offset;
    dest->size = size;
    dest->objtype = objtype;
    dest->rights = src->rights;
    return ERR_OK;
}

static errval_t cap_retype_ram(struct capability *dest, struct capability *src, 
                     enum cap_object_type objtype, size_t size, ptrdiff_t offset)
{
    errval_t err = ERR_OK;
    if (objtype == CAP_OBJECT_DEVICE) {
        err = CAP_ERR_RETYPE_SIBLING;
    } else {
        dest->base = src->base + offset;
        dest->size = size;
        dest->objtype = objtype;
        dest->rights = src->rights;
    }

    return err;
}

static errval_t cap_retype_frame(struct capability *dest, struct capability *src, 
                     enum cap_object_type objtype, size_t size, ptrdiff_t offset)
{
    errval_t err = ERR_OK;
    switch (objtype) {
        case CAP_OBJECT_DEVICE: 
        case CAP_OBJECT_RAM: {
            err = CAP_ERR_RETYPE_SUPER;
        } break;
        default: {
            dest->base = src->base + offset;
            dest->size = size;
            dest->objtype = objtype;
            dest->rights = src->rights;
        }
    }
    return err;
}

errval_t caps_retype(struct capability *dest, struct capability *src, 
                     enum cap_object_type objtype, size_t size, ptrdiff_t offset)
{
    errval_t err = 0;
    if (offset < 0) {
        err = CAP_ERR_NEGATIVE_OFFSET;
    } else if (size > src->size) {
        err = CAP_ERR_RETYPE_TOO_SMALL;
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
                err = CAP_ERR_INVALID_TYPE_OPERATION;
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
    dest->rights = src->rights;
    return ERR_OK;
}

errval_t caps_destroy(struct capability *dest)
{
    dest->base = 0;
    dest->size = 0;
    dest->objtype = CAP_OBJECT_NULL;
    dest->rights = 0;
    return ERR_OK;
}

errval_t caps_lookup_cap(struct capability *cnode, capaddr_t caddr, struct capability **ret_cap)
{
    errval_t err = ERR_OK;
    struct capability **caps = (struct capability **)(cnode->base + VSPACE_KERN_BASE);
    if (cnode->objtype == CAP_OBJECT_L1) {
        *ret_cap = caps[CAP_L1_OFFSET(caddr)];
    } else if (cnode->objtype == CAP_OBJECT_L2) {
        *ret_cap = caps[CAP_L2_OFFSET(caddr)];
    } else {
        err = CAP_ERR_INVALID_TYPE_OPERATION;
    }
    return err;
}

errval_t caps_write_cap(struct capability *cnode, capaddr_t caddr, struct capability *cap)
{
    errval_t err = ERR_OK;
    struct capability **caps = (struct capability **)(cnode->base + VSPACE_KERN_BASE);
    if (cnode->objtype == CAP_OBJECT_L1) {
        caps[CAP_L1_OFFSET(caddr)] = cap;
    } else if (cnode->objtype == CAP_OBJECT_L2) {
        caps[CAP_L2_OFFSET(caddr)] = cap;
    } else {
        err = CAP_ERR_INVALID_TYPE_OPERATION;
    }
    return err;
}

errval_t caps_create_vtable(void *table, size_t buflen, enum cap_object_type objtype,
                            bool readonly, struct capability *ret_cap)
{
    if (table == NULL) {
        return CAP_ERR_NULL_BUFFER;
    }

    if (buflen != VSPACE_BASE_PAGE_SIZE) {
        return CAP_ERR_OVERSIZED_VTABLE;
    }

    if (ret_cap == NULL) {
        return CAP_ERR_DEREF_NONPRESENT_CAP;
    }

    switch (objtype) {
        case CAP_OBJECT_VTL1:
        case CAP_OBJECT_VTL2:
        case CAP_OBJECT_VTL3:
        case CAP_OBJECT_VTL4: break;
        default: return CAP_ERR_INVALID_TYPE_OPERATION;
    }

    paddr_t addr;
    errval_t err = vtable_get_mapping(vtable_current(), table, &addr);
    if (err_is_fail(err)) {
        return err;
    }

    ret_cap->base = addr;
    ret_cap->objtype = objtype;
    ret_cap->rights = readonly ? CAP_RIGHTS_RDWR : CAP_RIGHTS_READ;
    ret_cap->size = buflen;

    return err;
}

errval_t caps_vmap(struct capability *dest, struct capability *src, ptrdiff_t off,
                   capslot_t slot, capslot_t nslots, int flags)
{
    if (dest == NULL || src == NULL) {
        return CAP_ERR_DEREF_NONPRESENT_CAP;
    }

    if (!(dest->rights & CAP_RIGHTS_RDWR)) {
        return CAP_ERR_WRITE_WITHOUT_PERM;
    }

    if (off < 0) {
        return CAP_ERR_NEGATIVE_OFFSET;
    }

    if (src->objtype != CAP_OBJECT_FRAME && src->size != VSPACE_BASE_PAGE_SIZE) {
        return CAP_ERR_INVALID_TYPE_OPERATION;
    }

    if (slot + nslots >= (1 << VSPACE_EXPECTED_PAGE_BITS)) {
        return CAP_ERR_SLOT_OOB;
    }

    errval_t err = ERR_OK;
    switch (dest->objtype) {
        case CAP_OBJECT_VTL1:
            if (off != 0) {
                return CAP_ERR_UNALIGNED_OFFSET;
            }

            if (src->objtype == CAP_OBJECT_VTL2) {
                err = vtable_set_ent((vtable_t)dest->base, 1, slot, src->base, src->size, flags);
            } else {
                err = CAP_ERR_INVALID_TYPE_OPERATION;
            }
        break;
        case CAP_OBJECT_VTL2:
            if (off % VSPACE_HUGE_PAGE_SIZE != 0 || src->size % VSPACE_HUGE_PAGE_SIZE != 0) {
                return CAP_ERR_UNALIGNED_OFFSET;
            }

            if (src->objtype == CAP_OBJECT_VTL3 || src->objtype == CAP_OBJECT_FRAME) {
                for (capslot_t s = slot; s < slot + nslots; ++s) {
                    err = vtable_set_ent((vtable_t)dest->base, 2, s, 
                            src->base + off + (s * VSPACE_HUGE_PAGE_SIZE),
                            VSPACE_HUGE_PAGE_SIZE, flags);
                    if (err_is_fail(err)) {
                        for (capslot_t c = slot; c < s; ++c) {
                            *((vtable_t)dest->base + c) = 0;
                        }
                        return err;
                    }
                }
            } else {
                err = CAP_ERR_INVALID_TYPE_OPERATION;
            }
        break;
        case CAP_OBJECT_VTL3:
            if (off % VSPACE_SUPER_PAGE_SIZE != 0 || src->size % VSPACE_SUPER_PAGE_SIZE != 0) {
                return CAP_ERR_UNALIGNED_OFFSET;
            }

            if (src->objtype == CAP_OBJECT_VTL4 || src->objtype == CAP_OBJECT_FRAME) {
                
                for (capslot_t s = slot; s < slot + nslots; ++s) {
                    err = vtable_set_ent((vtable_t)dest->base, 2, s, 
                            src->base + off + (s * VSPACE_SUPER_PAGE_SIZE),
                            VSPACE_SUPER_PAGE_SIZE, flags);
                    if (err_is_fail(err)) {
                        for (capslot_t c = slot; c < s; ++c) {
                            *((vtable_t)dest->base + c) = 0;
                        }
                        return err;
                    }
                }
            } else {
                err = CAP_ERR_INVALID_TYPE_OPERATION;
            }
        break;
        case CAP_OBJECT_VTL4:
            if (off % VSPACE_BASE_PAGE_SIZE != 0 || src->size % VSPACE_BASE_PAGE_SIZE != 0) {
                return CAP_ERR_UNALIGNED_OFFSET;
            }

            if (src->objtype == CAP_OBJECT_FRAME) {
                for (capslot_t s = slot; s < slot + nslots; ++s) {
                    err = vtable_set_ent((vtable_t)dest->base, 2, s, 
                            src->base + off + (s * VSPACE_BASE_PAGE_SIZE),
                            VSPACE_SUPER_PAGE_SIZE, flags);
                    if (err_is_fail(err)) {
                        for (capslot_t c = slot; c < s; ++c) {
                            *((vtable_t)dest->base + c) = 0;
                        }
                        return err;
                    }
                }
            } else {
                err = CAP_ERR_INVALID_TYPE_OPERATION;
            }
        break;
        default: err = CAP_ERR_INVALID_TYPE_OPERATION;
    }

    return err;
}