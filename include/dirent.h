#ifndef POSIX_DIRENT_H
#define POSIX_DIRENT_H 1

#include <posix/ino_t.h>

#define NAME_MAX 64

struct dirent {
    ino_t d_ino;
    char d_name[NAME_MAX];
};

struct dirstream;
typedef struct dirstream DIR;

int alphasort(const struct dirent **, const struct dirent **);
int closedir(DIR *);
int dirfd(DIR *);
DIR *fdopendir(int);
DIR *opendir(const char *);
struct dirent *readdir(DIR *);
int readdir_r(DIR *__restrict__, struct dirent *__restrict__,
              struct dirent **__restrict__);
void rewinddir(DIR *);
int scandir(const char *, struct dirent ***, int (*)(const struct dirent *),
            int (*)(const struct dirent **, const struct dirent **));
void seekdir(DIR *, long);
long telldir(DIR *);

#endif
