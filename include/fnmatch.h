#ifndef POSIX_FNMATCH_H
#define POSIX_FNMATCH_H 1

#define FNM_NOMATCH 1

#define FNM_PATHNAME 1
#define FNM_PERIOD   2
#define FNM_NOESCAPE 3

int fnmatch(const char *, const char *, int);

#endif
