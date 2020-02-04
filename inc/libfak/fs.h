#ifndef FAKIX_LIBFAK_FS_H
#define FAKIX_LIBFAK_FS_H 1

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <svr/inode.h>

typedef int32_t streamdesc_t;
typedef ssize_t streampos_t;

streamdesc_t fs_open_stream(inode_t inode) kernel_unfriendly;
void fs_close_stream(streamdesc_t stream) kernel_unfriendly;

size_t fs_stream_write(streamdesc_t stream, const void *src, size_t bytes) kernel_unfriendly;
size_t fs_stream_read(void *dest, streamdesc_t stream, size_t bytes) kernel_unfriendly;

streampos_t fs_stream_tell(streamdesc_t stream) kernel_unfriendly;
streampos_t fs_stream_seek(streamdesc_t stream, streampos_t newpos) kernel_unfriendly;

bool fs_stream_writedir(streamdesc_t dstream, const char *entname, inode_t inode) kernel_unfriendly;
bool fs_stream_readdir(char *out_entname, streamdesc_t dstream) kernel_unfriendly;

#endif