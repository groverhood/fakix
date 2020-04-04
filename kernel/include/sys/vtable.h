#ifndef KERNEL_SYS_VTABLE_H
#define KERNEL_SYS_VTABLE_H 1

#include <stddef.h>
#include <fakix/addr.h>
#include <fakix/vspace.h>
#include <fakix/errtype.h>

#define VTABLE_SLOT(a, l) VSPACE_TABLE_OFFSET(a, l, VSPACE_EXPECTED_PAGE_BITS, VSPACE_EXPECTED_OFFSET_BITS)

typedef size_t    vtent_t;
typedef capslot_t vtslot_t;
typedef vtent_t  *vtable_t;
/* Used for type checking flags. */
typedef int       vtlevel_t;

vtable_t vtable_current(void);
errval_t vtable_set_current(vtable_t newvt, vtable_t *ret_oldvt);

errval_t vtable_set_ent(vtable_t vtable, vtlevel_t level, vtslot_t slot, paddr_t paddr, size_t bytes, int flags);
errval_t vtable_set_flags(vtable_t vtable, vtlevel_t level, vtslot_t slot, int flags);

errval_t vtable_get_ent(vtable_t vtable, vtslot_t slot, vtent_t *ret_ent);
errval_t vtable_get_mapping(vtable_t vtable, vaddr_t vaddr, paddr_t *ret_paddr);

#endif