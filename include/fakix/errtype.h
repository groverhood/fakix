#ifndef FAKIX_ERRTYPE_H
#define FAKIX_ERRTYPE_H 1

#include <stdint.h>
#include <stdbool.h>
#include <fakix/errors.h>

typedef uint64_t errval_t;

static inline bool err_is_ok(errval_t err)
{
    return (err == ERR_OK);
}

static inline bool err_is_fail(errval_t err)
{
    return (err != ERR_OK);
}

const char *err_string(errval_t err);

#endif