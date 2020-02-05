/**
 *  === capability.h ===
 * 
 *  This header exposes the interface for capabilities, which are used for
 *  determining permissions for memory.
 * 
 *  A capability is a one-to-one mapping from subject to each object that it
 *  has ownership over. It describes the region that it occupies, as well as
 *  the permissible operations on data within the region. Capabilities are to
 *  be referenced in userland, so when a capability is referenced, it is a
 *  guarantee that it is describing permissions of the current process.
 * 
 *  Memory management is achieved through the usage of capabilities, which
 *  means that it is achieved almost entirely in userland, aside from the traps
 *  that are invoked to modify page table mappings and allocate pages to a 
 *  process.
 * 
 *  This header additionally exposes enums for architecture-specific objects.
 **/

#ifndef FAKIX_LIBFAK_CBL_H
#define FAKIX_LIBFAK_CBL_H 1

#include <stdint.h>
#include <stddef.h>

typedef uintptr_t capaddr_t;
typedef intptr_t capslot_t;
typedef int caprights_t;
typedef int caperr_t;

#define CAPRIGHT_WRITE (0x01)
#define CAPRIGHT_READ  (0x02)

/* Object descriptor type. */
enum cap_object_type {
	CAP_OBJ_L1C, /* Level 1 Capability. */
	CAP_OBJ_L2C, /* Level 2 Capability. */
	CAP_OBJ_RAM, /* Untyped RAM. */
	CAP_OBJ_FRAME, /* Page frame. */

	/* ARCH_x86_64-specific object types. */
	CAP_OBJ_PML4, /* PML4 table. */
	CAP_OBJ_PDP, /* Pagedir pointer table. */
	CAP_OBJ_PGD, /* Page directory. */
	CAP_OBJ_PTB, /* Page table. */
};

struct cnode {
	capaddr_t addr; /* [0:3]: L2 index
	                   [4:63]: L2 physical address (4K aligned) */
	capslot_t slot; /* L1 index. */
};

struct capability {
	struct cnode cnode;
	enum cap_object_type type;
	caprights_t rights; /* Access rights. */
};

caperr_t cbl_create_cnode(struct capability *out_cap, struct cnode *out_cnode);
caperr_t cbl_create_cap(struct capability dest, enum cap_object_type type, 
					caprights_t rights, size_t bytes);
caperr_t cbl_retype_cap(struct capability dest, enum cap_object_type newtype);

#endif