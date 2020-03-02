#ifndef FAKIX_PAGING_H
#define FAKIX_PAGING_H 1

#include <stddef.h>
#include <stdbool.h>
#include <attributes.h>
#include <fakix/addr.h>
#include <fakix/cspace.h>
#include <fakix/vspace.h>
#include <fakix/errtype.h>

typedef int paging_flags_t;

struct paging_state_vregion {
    bool mapped;
    vaddr_t base_addr;
    size_t size;

    struct paging_state_vregion *parent;
    struct paging_state_vregion *lf, *ri;
};

struct paging_state_vmap {
    struct paging_state_vregion header;

    capaddr_t frame;
    capaddr_t vnode_mapping_cap;
};

struct paging_state_vnode {
    capaddr_t vtln;
    /* A possibly more optimal structure would to be to declare this as a
       struct paging_state_vnode **, and allocate it lazily so that it could
       fit neatly within a page. */
    struct paging_state_vnode *vnodes[VSPACE_TABLE_ENTRIES];
};

struct paging_state {
    cnode_t l1_cnode; /* This is localized to each paging state to simplify
                         process creation. */

    struct paging_state_vregion vmap_tree;
    struct paging_state_vnode l1_vnode; /* This contains the VTL1 cap. */
};

/* All mapping functions take a RAM capability, and retype it to a frame while
   also creating a vspace mapping. */

errval_t paging_init_state(struct paging_state *st, vaddr_t start_addr,
                           capaddr_t pagedir, cnode_t l1);
errval_t paging_alloc(struct paging_state *st, size_t size, void **ret_addr);
errval_t paging_map_frame(struct paging_state *st, capaddr_t frame, 
                          paging_flags_t flags, void **ret_addr);
errval_t paging_map_fixed(struct paging_state *st, vaddr_t addr, capaddr_t frame,
                          paging_flags_t flags);

struct paging_state *paging_get_current_state(void);
errval_t paging_set_current_state(struct paging_state *st);

errval_t paging_alloc_current(size_t size, void **ret_addr);
errval_t paging_map_frame_current(capaddr_t frame, paging_flags_t flags, 
                                  void **ret_addr);
errval_t paging_map_fixed_current(vaddr_t addr, capaddr_t frame, 
                                  paging_flags_t flags);

/**
 *  Performs the following mapping operations.
 * 
 *  1. VTLn <- VTLn+1
 *  2. VTLn <- Frame
 **/
errval_t paging_map_vnode(capaddr_t dest, capslot_t dest_slot, capaddr_t src, 
                          size_t slots, paging_flags_t flags, capaddr_t *ret_map);

#endif