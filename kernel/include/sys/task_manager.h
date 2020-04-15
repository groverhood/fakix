#ifndef KERNEL_SYS_TASK_MANAGER_H
#define KERNEL_SYS_TASK_MANAGER_H 1

#include <elf/elf.h>
#include <cap/caps.h>
#include <fakix/tcb.h>

struct task_manager {
    tcb_handle_t tcb;
    struct capability *root;
    struct capability *self;
    struct capability *vroot;
    struct task_manager *next, *prev;
};

extern struct task_manager *task_current;

void task_context_switch(struct task_manager *manag);

errval_t task_create(void *buf, size_t buflen, struct capability *l1cnode,
                     struct capability *ret_cap);
                
errval_t task_init(Elf64_Ehdr *hdr, struct task_manager *tm, struct capability *(*alloc_fn)(void));

errval_t task_create_from_ramdisk(void *buf, size_t buflen, struct capability *l1cnode,
                                  uint8_t *rd, const char *rdname, struct capability *ret_cap);

#endif