#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "blocks.h"
#include "util.h"

void error(const char *str) {
	fprintf(stderr, "fatal error: %s\n", str);
	exit(1);
}

void perror_wrapper() {
	perror("fatal error");
	exit(1);
}

size_t conv_size_str(const char *str) {
	char *size_str = strdup(str);
	char *endptr;
	unsigned long size = strtoul(size_str, &endptr, 10);

	char units = *endptr;
	free(size_str);
	switch (units) {
	case '\0':
	case 'B':
		return size;
	case 'K':
		return size*1024;
	case 'M':
		return size*1024*1024;
	case 'G':
		return size*1024*1024*1024;
	default:
		return 0;
	}
}

char *genfilename(const char *basename, uint64_t num) {
	int numlen = 1;
	uint64_t n = num;
	while (n > 9) {
		n /= 10;
		numlen++;
	}

	char *filename = malloc(strlen(basename) + numlen + 6);
	sprintf(filename, "%"PRIu64".%s.blk", num, basename);
	return filename;
}

// TODO: wikipedia says this is inefficent; fix it
void adler32_partial(uint8_t *buf, size_t size, uint16_t *a, uint16_t *b) {
	for (size_t i = 0; i < size; i++) {
		*a = (*a + buf[i]) % 65521;
		*b = (*b + *a) % 65521;
	}
}

// this maybe should be in `blocks.c`, only used for one `qsort` call
int cmp_block_handler(const void * a, const void * b) {
	struct blk_hdr header_a = ((struct block_handler *)a)->header;
	struct blk_hdr header_b = ((struct block_handler *)b)->header;
	return header_a.index - header_b.index;
}
