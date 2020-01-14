#ifndef FAKIX_STDINT_H
#define FAKIX_STDINT_H

typedef signed char        int8_t;
typedef signed short       int16_t;
typedef signed int         int32_t;
typedef signed long long   int64_t;

typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

#define INTN_MIN(type) ((1 << (sizeof(type) * 8 - 1)))
#define INTN_MAX(type) (INTN_MIN(type) - 1)
#define UINTN_MAX(type) ((type)-1)

#define INT8_MIN  INTN_MIN(int8_t)
#define INT16_MIN INTN_MIN(int16_t)
#define INT32_MIN INTN_MIN(int32_t)
#define INT64_MIN INTN_MIN(int64_t)

#define INT8_MAX  INTN_MAX(int8_t)
#define INT16_MAX INTN_MAX(int16_t)
#define INT32_MAX INTN_MAX(int32_t)
#define INT64_MAX INTN_MAX(int64_t)

#define UINT8_MAX  UINTN_MAX(uint8_t)
#define UINT16_MAX UINTN_MAX(uint16_t)
#define UINT32_MAX UINTN_MAX(uint32_t)
#define UINT64_MAX UINTN_MAX(uint64_t)

#endif