#ifndef POSIX_GRP_H
#define POSIX_GRP_H 1

#include <posix/gid_t.h>
#include <types/size_t.h>

struct group {
    char  *gr_name;
    gid_t  gr_gid;
    char **gr_mem;
};

void endgrent(void);
struct group *getgrent(void);
struct group *getgrgid(gid_t);
int getgrgid_r(gid_t, struct group *, char *, size_t, struct group **);
struct group *getgrnam(const char *);
int getgrnam_r(const char *, struct group *, char *, size_t, struct group **);
void setgrent(void);

#endif
