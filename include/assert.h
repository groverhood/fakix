#ifndef STD_ASSERT_H
#define STD_ASSERT_H 1

#ifdef NDEBUG
#define assert(ignore) ((void)0)
#else
#define assert(expr) do {                           \
    if (!(expr)) { puts("Assertion failed: " #expr); \
	           exit(1); } while (0)
#endif

#endif
