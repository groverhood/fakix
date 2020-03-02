#ifndef UTIL_ARITH_H
#define UTIL_ARITH_H 1

#include <stdint.h>

#define divide_round_up(num, denom) \
    ({ \
        __auto_type n = (num); \
        __auto_type d = (denom); \
        ((n + d - 1) / d); \
    })

#endif