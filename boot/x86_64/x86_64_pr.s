#
# === x86_64_pr.s ===
#
# Entry point: protected_mode_entry
#
# Sets up the paging system, among other preparations, and then jumps to long
# mode.
#

# This code runs in 32-bit protected mode.
.code32

# Boot sections are not linked in upper memory, so this distinction must be
# made.
.section .boot

.globl protected_mode_entry
.type protected_mode_entry, @function
protected_mode_entry:
    # Check if long mode is available on this machine


    ljmp $0x08, $long_mode_entry