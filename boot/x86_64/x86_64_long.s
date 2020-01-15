#
# === x86_64_long.s ===
#
# Entry point: long_mode_entry
#
# Loads the %rax register with the kernel virtual address of fakix_start, and
# then calls it.
#

.section .text
#include "load.h"

.extern fakix_kern_start

.globl long_mode_entry
long_mode_entry:
    # Temporary. Remove this after verying the bootloader works.
    hlt
    movq $0xFFFF800000000000, %rax
    call *fakix_kern_start(%rax)

# In theory, this should be unreachable, as the kernel is tasked with coming to
# terms with its own demise.
.unreachable:
    hlt
    jmp .unreachable
