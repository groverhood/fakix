#ifndef FAKIX_UTIL_ALGO_H
#define FAKIX_UTIL_ALGO_H 1

#define min(a, b) ({ __auto_type _a = (a); __auto_type _b = (b); _a < _b ? _a : _b; })
#define max(a, b) ({ __auto_type _a = (a); __auto_type _b = (b); _a > _b ? _a : _b; })
#define div_rnd_up(n, d) ({ __auto_type _n = (n); __auto_type _d = (d); (_n + _d - 1) / (_d); })

#define add_safe(a, b, pres) !__builtin_add_overflow(a, b, pres)
#define sub_safe(a, b, pres) !__builtin_sub_overflow(a, b, pres)
#define mul_safe(a, b, pres) !__builtin_mul_overflow(a, b, pres)

#endif