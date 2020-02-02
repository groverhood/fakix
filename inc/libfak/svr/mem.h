#ifndef FAKIX_LIBFAK_SVR_MEM_H
#define FAKIX_LIBFAK_SVR_MEM_H 1

#include <stddef.h>
#include <stdint.h>
#include <util/list.h>
#include <alloc/slab.h>
#include <cbl.h>

/* Memory region descriptor. */
struct memory_region {
	struct list_elem elem; /* Regions list elem. */
	struct capability cap;
	uintptr_t base;
	size_t size;
};

struct memory_manager {
	struct slab_allocator mralloc; /* Memory region descriptor alloactor. */

	struct list regions;
};

#endif