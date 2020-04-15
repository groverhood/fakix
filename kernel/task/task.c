#include <sys/vtable.h>
#include <sys/task_manager.h>

struct task_manager *task_current = NULL;

errval_t task_create(void *buf, size_t buflen, struct capability *l1cnode,
                     struct capability *ret_cap)
{
    paddr_t base;
    errval_t err = vtable_get_mapping(vtable_current(), (vaddr_t)buf, &base);
    if (err_is_fail(err)) {
        return err;
    }

    if (buflen != (1 << TASK_BITS)) {

    }
    
    struct task_manager *task = buf;
    task->tcb = (tcb_handle_t)(task + 1);
    task->self = ret_cap;
    task->root = l1cnode;

    struct tcb_generic_shared *tcb = tcb_get_generic_shared(task->tcb); 
    tcb->disabled = true;
    tcb->fpu_trap = true;

    ret_cap->base = base;
    ret_cap->size = buflen;
    ret_cap->rights = CAP_RIGHTS_RDWR;
    ret_cap->objtype = CAP_OBJECT_TASK;

    return ERR_OK;
}