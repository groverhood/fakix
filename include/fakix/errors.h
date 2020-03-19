#ifndef FAKIX_ERRORS_H
#define FAKIX_ERRORS_H 1

#define ERR_OK 0
#define ERR_OK_ERRSTR "no errors encountered"
#define CAP_ERR_WRITE_ALLOCATED_CAP 1
#define CAP_ERR_WRITE_ALLOCATED_CAP_ERRSTR "attempted to write into an allocated capability"
#define CAP_ERR_DEREF_NONPRESENT_CAP 2
#define CAP_ERR_DEREF_NONPRESENT_CAP_ERRSTR "attempted to dereference a non-present capability"
#define CAP_ERR_WRITE_WITHOUT_PERM 3
#define CAP_ERR_WRITE_WITHOUT_PERM_ERRSTR "attempted to write into a capability without proper permissions"
#define CAP_ERR_RETYPE_SUPER 4
#define CAP_ERR_RETYPE_SUPER_ERRSTR "attempted to retype to a supertype"
#define CAP_ERR_RETYPE_SIBLING 5
#define CAP_ERR_RETYPE_SIBLING_ERRSTR "attempted to retype to a sibling type"
#define CAP_ERR_INVALID_TYPE_OPERATION 6
#define CAP_ERR_INVALID_TYPE_OPERATION_ERRSTR "attempted to perform a type-specific operation on a incompatible capability"
#define CAP_ERR_RETYPE_TOO_SMALL 7
#define CAP_ERR_RETYPE_TOO_SMALL_ERRSTR "the provided source capability is too small to perform a retype with the given parameters"
#define CAP_ERR_NEGATIVE_OFFSET 8
#define CAP_ERR_NEGATIVE_OFFSET_ERRSTR "the capability operation cannot be performed with a negative offset"
#define IDT_ERR_INVALID_VECTOR 9
#define IDT_ERR_INVALID_VECTOR_ERRSTR "attempted to set an invalid interrupt vector"
#define IDT_ERR_RESERVED_VECTOR 10
#define IDT_ERR_RESERVED_VECTOR_ERRSTR "attempted to write into a reserved interrupt vector"
#define IDT_ERR_KERNEL_ADDR 11
#define IDT_ERR_KERNEL_ADDR_ERRSTR "given a physical address that overlaps with the kernel address space"

#endif
