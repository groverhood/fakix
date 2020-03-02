
(define-errors
    (error-group 'ERR
        (success 'OK))
    (error-group 'CAP_ERR
        (error 'WRITE_ALLOCATED_CAP "attempted to write into an allocated capability")
        (error 'DEREF_NONPRESENT_CAP "attempted to dereference a non-present capability")
        (error 'WRITE_WITHOUT_PERM "attempted to write into a capability without proper permissions")
        (error 'RETYPE_SUPER "attempted to retype to a supertype")
        (error 'RETYPE_SIBLING "attempted to retype to a sibling type"))
        (error 'INVALID_TYPE_OPERATION "attempted to perform a type-specific operation on a incompatible capability")
    )