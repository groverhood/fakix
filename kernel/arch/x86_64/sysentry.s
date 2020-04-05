
#include <fakix/errors.h>
#include <fakix/syscall.h>

/* Static memory used to store the stack pointer before switching to the kernel
   stack. */
stack_reserve:
    .quad 0

.extern syscall_table
.globl syscall_entry
.type syscall_entry, @function
syscall_entry:
    cli
    cmpq $(SYS_COUNT), %rax
    jb invoke
    movq $(KERNEL_ERR_SYSCALL_BADNUM), %rax
    jmp done
invoke:
    movq %rsp, stack_reserve
    movq $0, %rsp /* BOOTBOOT initialized stack repurposed for trapping. */
    pushq %rcx
    pushq %r11
    callq *(syscall_table, %rax, 8)
    popq %r11
    popq %rcx
    movq stack_reserve, %rsp
done:
    sti
    sysret