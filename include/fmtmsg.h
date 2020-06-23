#ifndef POSIX_FMTMSG_H
#define POSIX_FMTMSG_H 1

#define MM_HARD     1
#define MM_SOFT     2
#define MM_FIRM     3
#define MM_APPL     4
#define MM_UTIL     5
#define MM_OPSYS    6
#define MM_RECOVER  7
#define MM_NRECOV   8
#define MM_HALT     9
#define MM_ERROR   10
#define MM_WARNING 11
#define MM_INFO    12
#define MM_NOSEV   13
#define MM_PRINT   14
#define MM_CONSOLE 15

#define MM_NULLLBL ((char *)0)
#define MM_NULLSEV 0
#define MM_NULLMC  0L
#define MM_NULLTXT ((char *)0)
#define MM_NULLACT ((char *)0)
#define MM_NULLTAG ((char *)0)

int fmtmsg(long mc, const char *, int sev, const char *,
	   const char *, const char *);

#define MM_OK    0
#define MM_NOTOK 1
#define MM_NOMSG 2
#define MM_NOCON 3

#endif
