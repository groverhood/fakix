#ifndef STD_CTYPE_H
#define STD_CTYPE_H 1

#include <posix/locale_t.h>

int isalnum(int chr);
int isalnum_l(int chr, locale_t loc);

int isalpha(int chr);
int isalpha_l(int chr, locale_t loc);

int isascii(int chr);

int isblank(int chr);
int isblank_l(int chr, locale_t loc);

int iscntrl(int chr);
int iscntrl_l(int chr, locale_t loc);

int isdigit(int chr);
int isdigit_l(int chr, locale_t loc);

int isgraph(int chr);
int isgraph_l(int chr, locale_t loc);

int islower(int chr);
int islower_l(int chr, locale_t loc);

int isprint(int chr);
int isprint_l(int chr, locale_t loc);

int ispunct(int chr);
int ispunct_l(int chr, locale_t loc);

int isspace(int chr);
int isspace_l(int chr, locale_t loc);

int isupper(int chr);
int isupper_l(int chr, locale_t loc);

int isxdigit(int chr);
int isxdigit_l(int chr, locale_t loc);

int toascii(int chr);

int tolower(int chr);
int tolower_l(int chr, locale_t loc);

int toupper(int chr);
int toupper_l(int chr, locale_t loc);

int _toupper(int chr);
#define _toupper(chr) toupper(chr)

int _tolower(int chr);
#define _tolower(chr) tolower(chr)

#endif
