#ifndef POSIX_INTTYPES_H
#define POSIX_INTTYPES_H 1

#include <stdint.h>
#include <types/wchar_t.h>

typedef struct imaxdiv {
    intmax_t quo;
    intmax_t rem;
} imaxdiv_t;

#define PRId8       "hhd"
#define PRIdLEAST8  "hhd"
#define PRIdFAST8   "d"
#define PRId16      "hd"
#define PRIdLEAST16 "hd"
#define PRIdFAST16  "d"
#define PRId32      "d"
#define PRIdLEAST32 "d"
#define PRIdFAST32  "ld"
#define PRId64      "ld"
#define PRIdLEAST64 "ld"
#define PRIdFAST64  "ld"
#define PRIdMAX     "lld"
#define PRIdPTR     "ld"

#define PRIi8       "hhi"
#define PRIiLEAST8  "hhi"
#define PRIiFAST8   "i"
#define PRIi16      "hi"
#define PRIiLEAST16 "hi"
#define PRIiFAST16  "i"
#define PRIi32      "i"
#define PRIiLEAST32 "i"
#define PRIiFAST32  "li"
#define PRIi64      "li"
#define PRIiLEAST64 "li"
#define PRIiFAST64  "li"
#define PRIiMAX     "lli"
#define PRIiPTR     "li"

#define PRIo8       "hho"
#define PRIoLEAST8  "hho"
#define PRIoFAST8   "o"
#define PRIo16      "ho"
#define PRIoLEAST16 "ho"
#define PRIoFAST16  "o"
#define PRIo32      "o"
#define PRIoLEAST32 "o"
#define PRIoFAST32  "lo"
#define PRIo64      "lo"
#define PRIoLEAST64 "lo"
#define PRIoFAST64  "lo"
#define PRIoMAX     "llo"
#define PRIoPTR     "lo"

#define PRIu8       "hhu"
#define PRIuLEAST8  "hhu"
#define PRIuFAST8   "u"
#define PRIu16      "hu"
#define PRIuLEAST16 "hu"
#define PRIuFAST16  "u"
#define PRIu32      "u"
#define PRIuLEAST32 "u"
#define PRIuFAST32  "lu"
#define PRIu64      "lu"
#define PRIuLEAST64 "lu"
#define PRIuFAST64  "lu"
#define PRIuMAX     "llu"
#define PRIuPTR     "lu"

#define PRIx8       "hhx"
#define PRIxLEAST8  "hhx"
#define PRIxFAST8   "x"
#define PRIx16      "hx"
#define PRIxLEAST16 "hx"
#define PRIxFAST16  "x"
#define PRIx32      "x"
#define PRIxLEAST32 "x"
#define PRIxFAST32  "lx"
#define PRIx64      "lx"
#define PRIxLEAST64 "lx"
#define PRIxFAST64  "lx"
#define PRIxMAX     "llx"
#define PRIxPTR     "lx"

#define PRIX8       "hhX"
#define PRIXLEAST8  "hhX"
#define PRIXFAST8   "X"
#define PRIX16      "hX"
#define PRIXLEAST16 "hX"
#define PRIXFAST16  "X"
#define PRIX32      "X"
#define PRIXLEAST32 "X"
#define PRIXFAST32  "lX"
#define PRIX64      "lX"
#define PRIXLEAST64 "lX"
#define PRIXFAST64  "lX"
#define PRIXMAX     "llX"
#define PRIXPTR     "lX"

/* SCANF */

#define SCNd8       "hhd"
#define SCNdLEAST8  "hhd"
#define SCNdFAST8   "d"
#define SCNd16      "hd"
#define SCNdLEAST16 "hd"
#define SCNdFAST16  "d"
#define SCNd32      "d"
#define SCNdLEAST32 "d"
#define SCNdFAST32  "ld"
#define SCNd64      "ld"
#define SCNdLEAST64 "ld"
#define SCNdFAST64  "ld"
#define SCNdMAX     "lld"
#define SCNdPTR     "ld"

#define SCNi8       "hhi"
#define SCNiLEAST8  "hhi"
#define SCNiFAST8   "i"
#define SCNi16      "hi"
#define SCNiLEAST16 "hi"
#define SCNiFAST16  "i"
#define SCNi32      "i"
#define SCNiLEAST32 "i"
#define SCNiFAST32  "li"
#define SCNi64      "li"
#define SCNiLEAST64 "li"
#define SCNiFAST64  "li"
#define SCNiMAX     "lli"
#define SCNiPTR     "li"

#define SCNo8       "hho"
#define SCNoLEAST8  "hho"
#define SCNoFAST8   "o"
#define SCNo16      "ho"
#define SCNoLEAST16 "ho"
#define SCNoFAST16  "o"
#define SCNo32      "o"
#define SCNoLEAST32 "o"
#define SCNoFAST32  "lo"
#define SCNo64      "lo"
#define SCNoLEAST64 "lo"
#define SCNoFAST64  "lo"
#define SCNoMAX     "llo"
#define SCNoPTR     "lo"

#define SCNu8       "hhu"
#define SCNuLEAST8  "hhu"
#define SCNuFAST8   "u"
#define SCNu16      "hu"
#define SCNuLEAST16 "hu"
#define SCNuFAST16  "u"
#define SCNu32      "u"
#define SCNuLEAST32 "u"
#define SCNuFAST32  "lu"
#define SCNu64      "lu"
#define SCNuLEAST64 "lu"
#define SCNuFAST64  "lu"
#define SCNuMAX     "llu"
#define SCNuPTR     "lu"

#define SCNx8       "hhx"
#define SCNxLEAST8  "hhx"
#define SCNxFAST8   "x"
#define SCNx16      "hx"
#define SCNxLEAST16 "hx"
#define SCNxFAST16  "x"
#define SCNx32      "x"
#define SCNxLEAST32 "x"
#define SCNxFAST32  "lx"
#define SCNx64      "lx"
#define SCNxLEAST64 "lx"
#define SCNxFAST64  "lx"
#define SCNxMAX     "llx"
#define SCNxPTR     "lx"

#endif
