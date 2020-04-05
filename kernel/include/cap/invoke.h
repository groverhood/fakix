#ifndef KERNEL_CAP_INVOKE_H
#define KERNEL_CAP_INVOKE_H 1

#include <cap/caps.h>
#include <fakix/errtype.h>

errval_t caps_invoke(capaddr_t cap, cap_invoke_t invocation, sysarg_t arg0, 
                     sysarg_t arg1, sysarg_t arg2, sysarg_t arg3, sysarg_t arg4);

#endif