#
# === x86_64_long.s ===
#
# Entry point: long_mode_entry
#
# Sets up the paging system, among other preparations, and then jumps to long
# mode.
#

# Load a physical address into an arbitrary destination as kernel virtual
# address.
.macro loadk addr, dest
    movq $-0x800000000000, \dest
    addq \addr\(), \dest
.endm

# The portable kernel entry point.
.extern fakix_main

# The FAKIX memory map, which has been populated in real mode.
.extern fakix_memmap

# How %rdi will be interpreted in fakix_main
# struct fakix_system_table {
#     struct fakix_memmap *mmap;
#     struct fakix_image_header *imghdr;
#     struct fakix_sdt *rsdt;
# };

.globl long_mode_entry
.type long_mode_entry, @function
long_mode_entry:
    subq $24, %rsp

    # Provide the FAKIX memory map as the first parameter to fakix_main
    loadk $fakix_memmap, (%rsp)
    loadk $fakix_image_header, 8(%rsp)
    loadk $fakix_rsdt, 16(%rsp)

    movq %rsp, %rdi
    # Load the entry point as a kernel virtual address.
    loadk $fakix_main, %rcx
    call %rcx

# In theory, this should be unreachable, as the kernel is tasked with coming to
# terms with its own demise.
.unreachable:
    hlt
    jmp .unreachable