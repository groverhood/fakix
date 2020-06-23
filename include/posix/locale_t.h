#ifndef POSIX_LOCALE_T_H
#define POSIX_LOCALE_T_H 1

typedef struct __locale {
  struct __locale_data *__locales[13]; /* 13 = __LC_LAST. */
  /* To increase the speed of this solution we add some special members.  */
  const unsigned short int *__ctype_b;
  const int *__ctype_tolower;
  const int *__ctype_toupper;
  /* Note: LC_ALL is not a valid index into this array.  */
  const char *__names[13];
} *locale_t;

#endif
