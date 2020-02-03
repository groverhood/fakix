#ifndef FAKIX_LIBFAK_MACRO_H
#define FAKIX_LIBFAK_MACRO_H 1

#define __macro_expand__(mac) mac
#define macro_expand(mac) __macro_expand__(mac)

#endif