#
# === x86_64_load.s ===
#
# Entry point: load_entry
#
# Scan devices for Fakix disk header, then load the rest of the boot module
# into memory.
#

.code16

.section .load

# We will be reading single sectors into this until we find the Fakix image
# header.
.extern fakix_image_header
# We will read the boot module into this
.extern fakix_boot_begin

.globl load_entry
.type load_entry, @function
load_entry:
    cli

    ljmp $0, $.dummy
.dummy:

    xorw %ax, %ax
    movw %ax, %ds
    movw %ax, %ss
    movw %ax, %es
    movl $fakix_real_stack, %esp

    sti

    # 80h marks the start of the hard disk devices, going up to FFh.
    movb $0x80, %dl

    # Search for the hard drive that contains the Fakix partition table, which
    # is tagged by a magic constant equivalent to the qword-aligned and
    # null-terminated string "Fakix!".
.check_device:
    call read_sector
    
    call check_hdr
    cmpw $8, %ax
    je load_fakix

    incb %dl
    jne .check_device

    hlt

# Load the Boot Module into memory.
load_fakix:
    # 42h is the BIOS int 13h opcode for 'Read Sectors' using logical block
    # addressing.
    movb $0x42, %ah
    # Read from the first sector
    movl $fakix_image_header, %ebx
    movl 0x44(%ebx), %ecx 
    pushl %ecx # LBA [32:63]
    movl 0x40(%ebx), %ecx
    pushl %ecx # LBA [0:31]
    pushw $fakix_boot
    pushw $0
    movw 0x48(%ebx), %cx
    pushw %cx
    pushw $0x10
    movw %sp, %si
    int $0x13
    jc .read_failure
    addl $0x10, %esp

    jmp fakix_boot_begin

.type check_hdr, @function
check_hdr:
    pushw %bx
    xorw %ax, %ax
    xorw %bx, %bx

.check_hdr_loop:
    movw %bx(fakix_image_header), %ax
    cmpw %bx(.fakix_magik), %ax
    jne .end_loop

    incw %bx
    cmpw $8, %bx
    jne .check_hdr_loop

.check_hdr_ret:
    movw %bx, %ax
    popw %bx
    ret

# Read a single 512-byte block of data into fakix_image_header from the start of
# the current hard drive.
.type read_sector, @function
read_sector:
    # 42h is the BIOS int 13h opcode for 'Read Sectors' using logical block
    # addressing.
    movb $0x42, %ah
    # Read from the first sector
    pushl $0 # LBA [32:63]
    pushl $0 # LBA [0:31]
    pushw $fakix_image_header
    pushw $0
    pushw $1
    pushw $0x10
    movw %sp, %si
    int $0x13
    jc .read_failure
    addl $0x10, %esp
    ret

.read_failure:
    hlt

.fakix_magik:
    .string "Fakix!"

bootloader_end:
.org 510 - bootloader_end
.word 0xAA55