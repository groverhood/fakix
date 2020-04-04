#include <sys/vtable.h>
#include <fakix/vspace.h>
#include <fakix/errors.h>

vtable_t vtable_current(void)
{
    uint64_t vt;
    asm ("movq %cr3, %rax" : "=a" (vt));
    return (vtable_t)(vt & ~0xfff);
}

errval_t vtable_set_current(vtable_t newvt, vtable_t *ret_oldvt)
{
    *ret_oldvt = vtable_current();
    asm volatile ("movq %rdi, %cr3" :: "D" (newvt));
    return ERR_OK;
}

static int flags_transform(int flags, vtlevel_t level)
{

}

errval_t vtable_set_ent(vtable_t vtable, vtlevel_t level, vtslot_t slot, paddr_t paddr, size_t bytes, int flags)
{
    if (vtable == NULL) {
        return KERNEL_ERR_VT_NULL_VT;
    }

    if (slot >= (1 << VSPACE_EXPECTED_PAGE_BITS)) {
        return KERNEL_ERR_VT_SLOT_OOB;
    }

    if (bytes == 0 || (bytes & 0xfff) != 0) {
        return KERNEL_ERR_VT_BYTES_ZERO_OR_UNALIGNED;
    }
    
    if (bytes != VSPACE_BASE_PAGE_SIZE && bytes != VSPACE_SUPER_PAGE_SIZE && bytes != VSPACE_HUGE_PAGE_SIZE) {
        return KERNEL_ERR_VT_INVALID_SIZE;
    }

    int vte_flags = flags_transform(flags, level);
    if (vte_flags == -1) {
        return KERNEL_ERR_VT_INVALID_FLAGS;
    }

    switch (level) {
        case 1:
            if (bytes == VSPACE_BASE_PAGE_SIZE) {
                vtable[slot] = (paddr | flags);
            } else {
                return KERNEL_ERR_VT_INVALID_SIZE;
            }
        break;
        case 2:
            if (bytes == VSPACE_HUGE_PAGE_SIZE || bytes == VSPACE_BASE_PAGE_SIZE) {
                vtable[slot] = (paddr | flags);
            } else {
                return KERNEL_ERR_VT_INVALID_SIZE;
            }
        break;
        case 3:
            if (bytes == VSPACE_SUPER_PAGE_SIZE || bytes == VSPACE_BASE_PAGE_SIZE) {
                vtable[slot] = (paddr | flags);
            } else {
                return KERNEL_ERR_VT_INVALID_SIZE;
            }
        break;
        case 4:
            if (bytes == VSPACE_BASE_PAGE_SIZE) {
                vtable[slot] = (paddr | flags);
            } else {
                return KERNEL_ERR_VT_INVALID_SIZE;
            }
        break;
    }

    return ERR_OK;
}

errval_t vtable_set_flags(vtable_t vtable, vtlevel_t level, vtslot_t slot, int flags)
{
    if (vtable == NULL) {
        return KERNEL_ERR_VT_NULL_VT;
    }

    if (slot >= (1 << VSPACE_EXPECTED_PAGE_BITS)) {
        return KERNEL_ERR_VT_SLOT_OOB;
    }

    int vte_flags = flags_transform(flags, level);
    if (vte_flags == -1) {
        return KERNEL_ERR_VT_INVALID_FLAGS;
    }

    vtable[slot] &= (~0xfff & ~INT64_MIN);
    vtable[slot] |= vte_flags;

    return ERR_OK;
}

errval_t vtable_get_ent(vtable_t vtable, vtslot_t slot, vtent_t *ret_ent)
{
    if (vtable == NULL) {
        return KERNEL_ERR_VT_NULL_VT;
    }

    if (slot >= (1 << VSPACE_EXPECTED_PAGE_BITS)) {
        return KERNEL_ERR_VT_SLOT_OOB;
    }

    *ret_ent = vtable[slot];

    return ERR_OK;
}

#define VTABLE_PS_BIT (1 << 7)

errval_t vtable_get_mapping(vtable_t vtable, vaddr_t vaddr, paddr_t *ret_paddr)
{
    vtent_t ent;
    errval_t err = vtable_get_ent(vtable, VTABLE_SLOT(vaddr, 1), &ent);
    if (err_is_fail(err)) {
        return err;
    }

    if (ent == 0) {
        return KERNEL_ERR_VT_NULL_VT;
    }

    vtable_t vtl2 = (vtable_t)(ent & ~0xfff);
    err = vtable_get_ent(vtl2, VTABLE_SLOT(vaddr, 2), &ent);
    if (err_is_fail(err)) {
        return err;
    }

    if (ent == 0) {
        return KERNEL_ERR_VT_NULL_VT;
    }

    if (ent & VTABLE_PS_BIT) {
        *ret_paddr = (ent & ~0xfff);
        return ERR_OK;
    }
    
    vtable_t vtl3 = (vtable_t)(ent & ~0xfff);
    err = vtable_get_ent(vtl3, VTABLE_SLOT(vaddr, 3), &ent);
    if (err_is_fail(err)) {
        return err;
    }

    if (ent == 0) {
        return KERNEL_ERR_VT_NULL_VT;
    }

    if (ent & VTABLE_PS_BIT) {
        *ret_paddr = (ent & ~0xfff);
        return ERR_OK;
    }

    vtable_t vtl4 = (vtable_t)(ent & ~0xfff);
    err = vtable_get_ent(vtl4, VTABLE_SLOT(vaddr, 4), &ent);
    if (err_is_fail(err)) {
        return err;
    }

    if (ent == 0) {
        return KERNEL_ERR_VT_NULL_VT;
    }

    *ret_paddr = (ent & ~0xfff);

    return err;
}