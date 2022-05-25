#pragma once

#include <stdio.h>
#include <stddef.h>
#include <inttypes.h>

// Print an error to stderr and exit with code 1
void error(const char *str);

// Wrapper around perror to act like error()
void perror_wrapper();

// Converts a string with byte units to bytes. ex: "10K" => 10240
size_t conv_size_str(const char *str);

// Return an allocated string of an archive's filename given a basename.
// ex: ("hello_world.c", 27) => "27.hello_world.c.blk"
char *genfilename(const char *basename, uint64_t num);

// Computes part of a Adler-32 hash. Can be used on multiple buffers to
// compute a single hash. a must be initalized to 1 and b must be intialized
// to 0
void adler32_partial(uint8_t *buf, size_t size, uint16_t *a, uint16_t *b);

// Computes the the Adler-32 hash of a buffer. Uses `adler32_part()` under the
// hood.
uint32_t adler32(uint8_t *buf, size_t size);

// Compares two `struct block_handler`s for `qsort()`
int cmp_block_handler(const void * a, const void * b);
