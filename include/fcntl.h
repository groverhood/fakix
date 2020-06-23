#ifndef POSIX_FCNTL_H
#define POSIX_FCNTL_H 1

#include <posix/pid_t.h>
#include <posix/off_t.h>
#include <posix/mode_t.h>
#include <posix/seekval.h>

#define F_DUPFD  1
#define F_DUPFD_CLOEXEC (F_DUPFD | FD_CLOEXEC)
#define F_GETFD  2
#define F_SETFD  3
#define F_GETFL  4
#define F_SETFL  5
#define F_GETLK  6
#define F_SETLK  7
#define F_SETLKW 8
#define F_GETOWN 9
#define F_SETOWN 10

#define FD_CLOEXEC 020

#define F_RDLCK 1
#define F_UNLCK 2
#define F_WRLCK 3

#define O_CREAT     0000001
#define O_DIRECTORY 0000002
#define O_EXCL      0000004
#define O_NOCTTY    0000010
#define O_CLOEXEC   FD_CLOEXEC
#define O_NOFOLLOW  0000040
#define O_TRUNC     0000100
#define O_TTY_INIT  0000200
#define O_APPEND    0000400
#define O_DSYNC     0001000
#define O_NONBLOCK  0002000
#define O_RSYNC     0004000
#define O_SYNC      0010000
#define O_ACCMODE   0020000
#define O_EXEC      0040000
#define O_RDONLY    0100000
#define O_RDWR      0200000
#define O_SEARCH    O_EXEC
#define O_WRONLY    0400000

#define AT_FDCWD           -1
#define AT_EACCESS         -1
#define AT_SYMLINK_FOLLOW  -1
#define AT_SYMLINK_NOFOLOW -1
#define AT_REMOVEDIR       -1

#define POSIX_FADV_NORMAL 0
#define POSIX_FADV_DONTNEED 1
#define POSIX_FADV_NOREUSE 2
#define POSIX_FADV_RANDOM 3
#define POSIX_FADV_SEQUENTIAL 4
#define POSIX_FADV_WILLNEED 5

struct flock {
    short l_type;
    short l_whence;
    off_t l_start;
    off_t l_len;
    pid_t l_pid;
};

int creat(const char *fname, mode_t mod);
int fcntl(int fd, int cmd, ...);

int open(const char *fname, int oflags, ...);
int _open_nmode(const char *fname, int oflags);
int _open_wmode(const char *fname, int oflags, mode_t mod);
#define __open_sel_(_3, _2, _1, N, ...) _openat ## N 
#define __open_sel(...) __openat_sel_(__VA_ARGS__, wmode, nmode, invalid, invalid)
#define open(fname, oflags, ...) \
    __open_sel(fname, oflags, ## __VA_ARGS__)(fname, oflags, ## __VA_ARGS__)

int openat(int dirfd, const char *fname, int oflags, ...);
int _openat_nmode(int dirfd, const char *fname, int oflags);
int _openat_wmode(int dirfd, const char *fname, int oflags, mode_t mode);
#define __openat_sel_(_4, _3, _2, _1, N, ...) _openat ## N 
#define __openat_sel(...) __openat_sel_(__VA_ARGS__, wmode, nmode, invalid, invalid, invalid)
#define openat(dirfd, fname, oflags, ...) \
    __openat_sel(dirfd, fname, oflags, ## __VA_ARGS__)(dirfd, fname, oflags, ## __VA_ARGS__)

int posix_fadvise(int fd, off_t, off_t, int);
int posix_fallocate(int fd, off_t, off_t);

#endif
