
(define-errors
    (error-group "ERR"
        (success "OK"))
    (error-group "CAP_ERR"
        (error "NULL_BUFFER" "provided a null pointer to a capability")
        (error "OVERSIZED_VTABLE" "provided a vtable buffer larger than 4K bytes to vtable_create()")
        (error "WRITE_ALLOCATED_CAP" "attempted to write into an allocated capability")
        (error "DEREF_NONPRESENT_CAP" "attempted to dereference a non-present capability")
        (error "WRITE_WITHOUT_PERM" "attempted to write into a capability without proper permissions")
        (error "RETYPE_SUPER" "attempted to retype to a supertype")
        (error "RETYPE_SIBLING" "attempted to retype to a sibling type")
        (error "INVALID_TYPE_OPERATION" "attempted to perform a type-specific operation on a incompatible capability")
        (error "RETYPE_TOO_SMALL" "the provided source capability is too small to perform a retype with the given parameters")
        (error "NEGATIVE_OFFSET" "the capability operation cannot be performed with a negative offset")
        (error "UNALIGNED_OFFSET" "the offset provided is not aligned to a huge/super/base page boundary")
        (error "SLOT_OOB" "slot or slot range provided is out of bounds"))
    (error-group "IDT_ERR"
        (error "INVALID_VECTOR" "attempted to set an invalid interrupt vector")
        (error "RESERVED_VECTOR" "attempted to write into a reserved interrupt vector")
        (error "KERNEL_ADDR" "given a physical address that overlaps with the kernel address space"))
    (error-group "KERNEL_ERR"
        (error "VT_NULL_VT" "attempted to provide a null VTable")
        (error "VT_INVALID_FLAGS" "provided invalid flags")
        (error "VT_SLOT_OOB" "provided an out-of-bounds slot")
        (error "VT_BYTES_ZERO_OR_UNALIGNED" "provided a region containing no or unaligned memory")
        (error "VT_INVALID_SIZE" "provided an aligned size that wasn't valid for a single entry")))