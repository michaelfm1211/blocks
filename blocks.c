#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>
#include <getopt.h>
#include "blocks.h"
#include "util.h"

void split_file(const char *filename, size_t blocksize, const char
	*output) {
	FILE *file = fopen(filename, "rb");
	
	// create mostly equally sized blocks, although not strictly needed
	fseek(file, 0, SEEK_END);
	size_t filesize = ftell(file);
	fseek(file, 0, SEEK_SET);
	uint64_t total_blocks = filesize/blocksize;
	if (filesize%blocksize != 0) total_blocks++;

	if (output == NULL) output = filename;
	uint8_t *buf = malloc(READING_SIZE);
	uint64_t nread, i = 0;
	while ((nread = fread(buf, 1, blocksize, file)) != 0) {
		char *out_filename = genfilename(output, i);
		FILE *out = fopen(out_filename, "wb");
		free(out_filename);

		struct blk_hdr header;
		header.magic = BLK_MAGIC_V1;
		header.flags = 0;
		header.total_blocks = total_blocks;
		header.index = i;
		header.data_size = nread;
		header.data_hash = adler32(buf, nread);

		fwrite(&header, sizeof(struct blk_hdr), 1, out);
		fwrite(buf, nread, 1, out);
		i++;
	}
	free(buf);

	if (ferror(file))
		error("Incomplete archive. Error while reading file.");
}

void recombine_files(int numfiles, char *filenames[], const char
	*output) {
	// for each file, get the header and fd and put them in an array
	struct block_handler *handle_arr = NULL;
	size_t handle_arr_i = 0;
	FILE *file = NULL;
	for (int i = 0; i < numfiles; i++) {
		if ((file = fopen(filenames[i], "rb")) == NULL) break;
		struct blk_hdr header;
		if ((fread(&header, sizeof(struct blk_hdr), 1, file)) == 0)
			break;

		if (header.magic != BLK_MAGIC_V1) {
			free(handle_arr);
			error("Operation incomplete. File has invalid magic.");
		}

		if (handle_arr == NULL) {
			handle_arr = malloc(header.total_blocks * sizeof(
				struct block_handler));
		}
		struct block_handler handler = {.header = header,
			.file = file, .filename = filenames[i]};
		handle_arr[handle_arr_i++] = handler;
	}

	if (file == NULL) perror_wrapper();
	else if (ferror(file)) {
		free(handle_arr);
		error("Operation incomplete. Error while reading file.");
	} else if (feof(file)) {
		free(handle_arr);
		error("Operation incomplete. Unexpected EOF.");
	}

	// handle_arr may be out of order, so we must sort it.
	qsort(handle_arr, handle_arr_i, sizeof(struct block_handler),
		cmp_block_handler);

	if (output == NULL) output = DEFAULT_OUTPUT_NAME;
	FILE *out = fopen(output, "wb");

	// take the data struct, write to file and compute adler32 along the
	// way, then check it at the end
	uint8_t *buf = malloc(READING_SIZE);
	for (size_t i = 0; i < handle_arr_i; i++) {
		FILE *file = handle_arr[i].file;
		uint16_t a = 1, b = 0;
		size_t nread;
		while ((nread = fread(buf, 1, READING_SIZE, file)) != 0) {
			adler32_partial(buf, nread, &a, &b);
			fwrite(buf, nread, 1, out);
		}

		uint32_t hash = ((b << 16) | a);
		if (hash != handle_arr[i].header.data_hash) {
			fprintf(stderr, "warning: mismatching hash for %s\n",
				handle_arr[i].filename);
		}
	}

	free(buf);
	free(handle_arr);
}

int main(int argc, char *argv[]) {
	if (argc < 2) error(USAGE);

	bool split_flag = false, recombine_flag = false;
	size_t blocksize = 8 * 1024 * 1024;
	char *output = NULL;
	char ch;
	while ((ch = getopt(argc, argv, ":srz:o:")) != -1) {
		switch (ch) {
		case 's':
			split_flag = true;
			break;
		case 'r':
			recombine_flag = true;
			break;
		case 'z':
			blocksize = conv_size_str(optarg);
			if (blocksize == 0) error("-z flag is not a valid size");
			break;
		case 'o':
			output = optarg;
			break;
		case ':':
		case '?':
			error(USAGE);
			break;
		}
	}

	if (argc-optind == 0) error(USAGE);

	if (split_flag) {
		if (recombine_flag) error(USAGE);
		if (argc-optind > 1) error("cannot split multiple files");
		split_file(argv[optind], blocksize, output);
	} else if (recombine_flag) {
		if (argc-optind < 1) error(USAGE);
		recombine_files(argc-optind, argv+optind, output);
	} else {
		error(USAGE);
	}

	return 0;
}
