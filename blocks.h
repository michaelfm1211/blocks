#pragma once

#include <stdio.h>
#include <stdint.h>

#define USAGE "usage: blocks {-s | -r} [-v] [-z size] [-o filename] files ..."
#define VERSION "v1.0.0"
// default reading size is 1,048,576 bytes and default output name when
// using with -r flag is "output.bin". both can be changed at compile time
#define READING_SIZE 1024*1024
#define DEFAULT_OUTPUT_NAME "output.bin"

struct __attribute__((__packed__)) blk_hdr {
	uint32_t magic;		// magic that depends on version:
				// v1: 0xB10C0001
	uint32_t flags;		// currently not used

	// fields specific to the block chain
	uint64_t total_blocks;	// number of blocks in blockchain
	
	// fields specific to this block
	uint64_t index;		// index of this block
	uint64_t data_size;	// size of this block
	uint32_t data_hash;	// hash of the data
};

#define BLK_MAGIC_V1 0xB10C0001
#define BLK_FLAGS_NONE 0x0

struct block_handler {
	struct blk_hdr header;
	FILE *file;
	char *filename;
};
