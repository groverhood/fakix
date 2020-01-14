#
# === x86_64_real.s ===
#
# Entry point: real_mode_entry
#
# Uses BIOS goodies to provide the Kernel process with ample information, then
# jumps to protected mode.
#

# This code runs in 16-bit real mode.
.code16

.section .boot

.extern protected_mode_entry

.globl real_mode_entry
.type real_mode_entry, @function
real_mode_entry:


    ljmp $0x8, $protected_mode_entry