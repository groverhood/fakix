#ifndef FAKIX_UTIL_BITMAP_H
#define FAKIX_UTIL_BITMAP_H 1

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <util/algo.h>

#define BITMAP_NPOS ((size_t)-1)

typedef bool bit;

struct bitmap;

struct bitmap *bitmap_create(size_t bits);
struct bitmap *bitmap_create_buffer(size_t bits, void *buffer, size_t size);

size_t bitmap_size(struct bitmap *bm);

static inline size_t bitmap_bufsize(size_t bits)
{
	return div_rnd_up(bits, 8);
}

void bitmap_set(struct bitmap *bm, size_t start, size_t nbits, bit b);
void bitmap_flip(struct bitmap *bm, size_t start, size_t nbits);
size_t bitmap_scan(struct bitmap *bm, size_t start, size_t nbits, bit b);
size_t bitmap_scan_and_flip(struct bitmap *bm, size_t start, size_t nbits, bit b);
bool bitmap_all(struct bitmap *bm, size_t start, size_t nbits, bit b);


#endif