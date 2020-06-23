#ifndef POSIX_AIO_H
#define POSIX_AIO_H 1

typedef signed long off_t;
typedef signed long ssize_t;

#include <sys/time.h>
#include <sys/signal.h>

struct aiocb {
    int             aio_fildes;     // File descriptor. 
    off_t           aio_offset;     // File offset. 
    volatile void  *aio_buf;        // Location of buffer. 
    size_t          aio_nbytes;     // Length of transfer. 
    int             aio_reqprio;    // Request priority offset. 
    struct sigevent aio_sigevent;   // Signal number and value. 
    int             aio_lio_opcode; // Operation to be performed.
    int             __aio_error;
};

#define AIO_ALLDONE     0
#define AIO_CANCELED    1
#define AIO_NOTCANCELED 2

#define LIO_NOP 0x00000000
#define LIO_NOWAIT (0x1 << 0)
#define LIO_WAIT (0x1 << 1)
#define LIO_READ (0x1 << 2)
#define LIO_WRITE (0x1 << 3)

int aio_cancel(int, struct aiocb *);
static inline int aios_error(struct aiocb *aiocb)
{
    return aiocb->__aio_error;
}

int aio_fsync(int, struct aiocb *);
int aio_read(struct aiocb *);
ssize_t aio_return(struct aiocb *);
int aio_suspend(const struct aiocb *const [], int,
		const struct timespec *);
int aio_write(struct aiocb *);
int lio_listio(int, struct aiocb *__restrict__ const [__restrict_arr], int,
	       struct sigevent *__restrict__);

#endif
