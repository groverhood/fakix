#ifndef FAKIX_ERRORS_H
#define FAKIX_ERRORS_H 1

#define ERR_OK 0
#define ERR_OK_ERRSTR "no errors encountered"
#define CAP_ERR_NULL_BUFFER 1
#define CAP_ERR_NULL_BUFFER_ERRSTR "provided a null pointer to a capability"
#define CAP_ERR_OVERSIZED_VTABLE 2
#define CAP_ERR_OVERSIZED_VTABLE_ERRSTR "provided a vtable buffer larger than 4K bytes to vtable_create()"
#define CAP_ERR_WRITE_ALLOCATED_CAP 3
#define CAP_ERR_WRITE_ALLOCATED_CAP_ERRSTR "attempted to write into an allocated capability"
#define CAP_ERR_DEREF_NONPRESENT_CAP 4
#define CAP_ERR_DEREF_NONPRESENT_CAP_ERRSTR "attempted to dereference a non-present capability"
#define CAP_ERR_WRITE_WITHOUT_PERM 5
#define CAP_ERR_WRITE_WITHOUT_PERM_ERRSTR "attempted to write into a capability without proper permissions"
#define CAP_ERR_RETYPE_SUPER 6
#define CAP_ERR_RETYPE_SUPER_ERRSTR "attempted to retype to a supertype"
#define CAP_ERR_RETYPE_SIBLING 7
#define CAP_ERR_RETYPE_SIBLING_ERRSTR "attempted to retype to a sibling type"
#define CAP_ERR_INVALID_TYPE_OPERATION 8
#define CAP_ERR_INVALID_TYPE_OPERATION_ERRSTR "attempted to perform a type-specific operation on a incompatible capability"
#define CAP_ERR_RETYPE_TOO_SMALL 9
#define CAP_ERR_RETYPE_TOO_SMALL_ERRSTR "the provided source capability is too small to perform a retype with the given parameters"
#define CAP_ERR_NEGATIVE_OFFSET 10
#define CAP_ERR_NEGATIVE_OFFSET_ERRSTR "the capability operation cannot be performed with a negative offset"
#define CAP_ERR_UNALIGNED_OFFSET 11
#define CAP_ERR_UNALIGNED_OFFSET_ERRSTR "the offset provided is not aligned to a huge/super/base page boundary"
#define CAP_ERR_SLOT_OOB 12
#define CAP_ERR_SLOT_OOB_ERRSTR "slot or slot range provided is out of bounds"
#define IDT_ERR_INVALID_VECTOR 13
#define IDT_ERR_INVALID_VECTOR_ERRSTR "attempted to set an invalid interrupt vector"
#define IDT_ERR_RESERVED_VECTOR 14
#define IDT_ERR_RESERVED_VECTOR_ERRSTR "attempted to write into a reserved interrupt vector"
#define IDT_ERR_KERNEL_ADDR 15
#define IDT_ERR_KERNEL_ADDR_ERRSTR "given a physical address that overlaps with the kernel address space"
#define KERNEL_ERR_VT_NULL_VT 16
#define KERNEL_ERR_VT_NULL_VT_ERRSTR "attempted to provide a null VTable"
#define KERNEL_ERR_VT_INVALID_FLAGS 17
#define KERNEL_ERR_VT_INVALID_FLAGS_ERRSTR "provided invalid flags"
#define KERNEL_ERR_VT_SLOT_OOB 18
#define KERNEL_ERR_VT_SLOT_OOB_ERRSTR "provided an out-of-bounds slot"
#define KERNEL_ERR_VT_BYTES_ZERO_OR_UNALIGNED 19
#define KERNEL_ERR_VT_BYTES_ZERO_OR_UNALIGNED_ERRSTR "provided a region containing no or unaligned memory"
#define KERNEL_ERR_VT_INVALID_SIZE 20
#define KERNEL_ERR_VT_INVALID_SIZE_ERRSTR "provided an aligned size that wasn't valid for a single entry"

#endif
