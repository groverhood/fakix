#
# === x86_64_long.s ===
#
# Entry point: long_mode_entry
#
# Loads the %rax register with the kernel virtual address of fakix_start, and
# then calls it.
#

.section .text

.extern fakix_kern_start
.extern fakix_gdt_kern_data
.extern fakix_pr_stack

.virtual_entry_address:
    .quad 0

.globl long_mode_entry
long_mode_entry:
    movw $fakix_gdt_kern_data, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    movq $0xFFFF800000000000, %rsp
    addq $fakix_pr_stack, %rsp

    movq $0xFFFF800000000000, %rax
    addq $fakix_kern_start, %rax
    movq %rax, .virtual_entry_address
    call *(.virtual_entry_address)

# In theory, this should be unreachable, as the kernel is tasked with coming to
# terms with its own demise.
.unreachable:
    hlt
    jmp .unreachable
