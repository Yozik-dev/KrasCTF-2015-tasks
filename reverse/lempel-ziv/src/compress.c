#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#define BITS 15
#define HASHING_SHIFT (BITS - 8)
#define MAX_VALUE ((1 << BITS) - 1)
#define MAX_CODE (MAX_VALUE - 1)

#define TABLE_SIZE 32771
#define DECODE_STACK_SIZE TABLE_SIZE

#define OUTPUT_BUFFER_SIZE 4096

struct compress_cntx;

typedef struct output_buffer {
	uint8_t data[OUTPUT_BUFFER_SIZE];
	uint32_t crnt_indx;
	void (*sync)(struct compress_cntx *);
} output_buffer_t;

typedef struct maped_area {
	uint8_t *map;
	uint32_t size;
	uint32_t crnt_indx;
} maped_area_t;

typedef struct compress_cntx {
	void (*put)(int value, struct compress_cntx *cntx);
	unsigned int (*get)(struct compress_cntx *cntx);

	int *code_value;
	unsigned int *prefix_code;
	unsigned char *append_character;
	unsigned char decode_stack[DECODE_STACK_SIZE];

	output_buffer_t buffer;
	maped_area_t file_map;
	int in_fd, out_fd;
} compress_cntx_t;

#ifdef _DEBUG

static unsigned int input_code(compress_cntx_t *cntx)
{
	unsigned int return_value;
	static int input_bit_count = 0;
	static unsigned long input_bit_buffer = 0L;

	while (input_bit_count <= 24) {
		input_bit_buffer |=
			(unsigned long) cntx->get(cntx) << (24 - input_bit_count);
		input_bit_count += 8;
	}

	return_value = input_bit_buffer >> (32-BITS);
	input_bit_buffer <<= BITS;
	input_bit_count -= BITS;

	return return_value;
}

#endif

static void output_code(unsigned int code, compress_cntx_t *cntx)
{
	static int output_bit_count = 0;
	static unsigned long output_bit_buffer = 0L;

	output_bit_buffer |=
		(unsigned long) code << (32 - BITS - output_bit_count);
	output_bit_count += BITS;

	while (output_bit_count >= 8) {
		cntx->put(output_bit_buffer >> 24, cntx);
		output_bit_buffer <<= 8;
		output_bit_count -= 8;
	}
}

static int find_match(int hash_prefix, unsigned int hash_character,
		      compress_cntx_t *cntx)
{
	int index;
	int offset;

	index = (hash_character << HASHING_SHIFT) ^ hash_prefix;
	if (index == 0)
		offset = 1;
	else
		offset = TABLE_SIZE - index;

	while (1) {
		if (cntx->code_value[index] == -1)
			return index;
		if (cntx->prefix_code[index] == hash_prefix
		    && cntx->append_character[index] == hash_character)
			return index;
		index -= offset;
		if (index < 0)
			index += TABLE_SIZE;
	}
}

static void compress(compress_cntx_t *cntx)
{
	unsigned int next_code = 256;
	unsigned int character;
	unsigned int string_code;
	unsigned int index;
	int i;

	for (i=0; i < TABLE_SIZE; i++)
		cntx->code_value[i] = -1;
	string_code = cntx->get(cntx);

	while ((character = cntx->get(cntx)) != (unsigned) EOF) {
		index = find_match(string_code, character, cntx);
		if (cntx->code_value[index] != -1) {
			string_code = cntx->code_value[index];
		} else {
			if (next_code <= MAX_CODE) {
				cntx->code_value[index] = next_code++;
				cntx->prefix_code[index] = string_code;
				cntx->append_character[index] = character;
			}
			output_code(string_code, cntx);
			string_code = character;
		}
	}

	output_code(string_code, cntx);
	output_code(MAX_VALUE, cntx);
	output_code(0, cntx);
}

#ifdef _DEBUG

static unsigned char *decode_string(unsigned char *buffer, unsigned int code,
				    compress_cntx_t *cntx)
{
	int i = 0;

	while (code > 255) {
		*buffer++ = cntx->append_character[code];
		code = cntx->prefix_code[code];
		if (++i == DECODE_STACK_SIZE)
			break;
	}

	*buffer = code;
	return buffer;
}

static void expand(compress_cntx_t *cntx)
{
	unsigned int next_code = 256;
	unsigned int new_code;
	unsigned int old_code;
	int character;
	unsigned char *string = NULL;
	unsigned char *_decode_stack = cntx->decode_stack;

	old_code = character = input_code(cntx);
	cntx->put(old_code, cntx);

	while ((new_code = input_code(cntx)) != (MAX_VALUE)) {
		if (new_code >= next_code) {
			*_decode_stack = character;
			string = decode_string(_decode_stack + 1, old_code, cntx);
		} else {
			string = decode_string(_decode_stack, new_code, cntx);
		}

		character = *string;
		while (string >= _decode_stack)
			cntx->put(*string--, cntx);

		if (next_code <= MAX_CODE) {
			cntx->prefix_code[next_code] = old_code;
			cntx->append_character[next_code] = character;
			next_code++;
		}
		old_code = new_code;
	}
}

#endif

static void cached_put(int value, compress_cntx_t *cntx)
{
	if (cntx->buffer.crnt_indx >= OUTPUT_BUFFER_SIZE) {
		write(cntx->out_fd, cntx->buffer.data, OUTPUT_BUFFER_SIZE);
		cntx->buffer.crnt_indx = 0;
	}
	cntx->buffer.data[cntx->buffer.crnt_indx++] = (uint8_t) value;
}

static unsigned int maped_get(compress_cntx_t *cntx)
{
	if (cntx->file_map.crnt_indx < cntx->file_map.size)
		return (unsigned)
			cntx->file_map.map[cntx->file_map.crnt_indx++];
	return (unsigned) EOF;
}

static void sync_cached_buffer(compress_cntx_t *cntx)
{
	if (cntx->buffer.crnt_indx > 0)
		write(cntx->out_fd, cntx->buffer.data,
		      cntx->buffer.crnt_indx);
}

static compress_cntx_t *context_alloc()
{
	compress_cntx_t *cntx = malloc(sizeof(compress_cntx_t));
	if (cntx == NULL)
		return NULL;

	cntx->code_value = malloc(TABLE_SIZE * sizeof(unsigned int));
	if (cntx->code_value == NULL)
		return NULL;

	cntx->prefix_code = malloc(TABLE_SIZE * sizeof(unsigned int));
	if (cntx->prefix_code == NULL)
		return NULL;

	cntx->append_character = malloc(TABLE_SIZE * sizeof(unsigned char));
	if (cntx->append_character == NULL)
		return NULL;

	memset(cntx->buffer.data, 0, OUTPUT_BUFFER_SIZE);
	cntx->buffer.sync = sync_cached_buffer;
	cntx->buffer.crnt_indx = 0;

	cntx->put = cached_put;
	cntx->get = maped_get;

	return cntx;
}

static void context_free(compress_cntx_t *cntx)
{
	if (cntx == NULL)
		return;
	if (cntx->code_value != NULL)
		free(cntx->code_value);
	if (cntx->prefix_code != NULL)
		free(cntx->prefix_code);
	if (cntx->append_character != NULL)
		free(cntx->append_character);
	if (cntx->file_map.map != MAP_FAILED)
		munmap(cntx->file_map.map, cntx->file_map.size);
	if (cntx->in_fd != -1)
		close(cntx->in_fd);
	if (cntx->out_fd != -1) {
		cntx->buffer.sync(cntx);
		close(cntx->out_fd);
	}
	free(cntx);
}

static int open_files(compress_cntx_t *cntx, const char *in_file,
		      const char *out_file)
{
	cntx->in_fd = open(in_file, O_RDONLY);
	if (cntx->in_fd == -1) {
		perror("failed to open input file");
		return -1;
	}

	cntx->out_fd = open(out_file, O_WRONLY | O_CREAT,
			    S_IRUSR | S_IWUSR);
	if (cntx->out_fd == -1) {
		perror("failed to open output file");
		return -1;
	}

	return 0;
}

static int map_file_into_mem(compress_cntx_t *cntx)
{
	struct stat file_stat;
	memset(&file_stat, 0, sizeof(file_stat));
	if (fstat(cntx->in_fd, &file_stat) != 0)
		return -1;
	cntx->file_map.size = file_stat.st_size;
	cntx->file_map.map = mmap(NULL, cntx->file_map.size, PROT_READ,
				  MAP_PRIVATE, cntx->in_fd, 0);
	if (cntx->file_map.map == MAP_FAILED)
		return -1;
	cntx->file_map.crnt_indx = 0;
	return 0;
}

static inline int usage(const char *prog_name)
{
	fprintf(stderr, "%s [-c | -d] input output\n", prog_name);
	return 1;
}

int main(int argc, char *argv[])
{
	compress_cntx_t *cntx = NULL;

	if (argc != 4)
		return usage(argv[0]);

	if ((cntx = context_alloc()) == NULL) {
		perror("failed to allocate context memory");
		context_free(cntx);
		return 1;
	}

	if (open_files(cntx, argv[2], argv[3]) != 0) {
		context_free(cntx);
		return 1;
	}

	if (map_file_into_mem(cntx) != 0) {
		perror("failed to map file content into memory");
		context_free(cntx);
		return 1;
	}

	if (argv[1][0] == '-' && argv[1][1] == 'c') {
		compress(cntx);
	} else if (argv[1][0] == '-' && argv[1][1] == 'd') {
#ifdef _DEBUG
		expand(cntx);
#else
		fprintf(stderr, "not implemented\n");
#endif
	} else {
		return usage(argv[0]);
	}

	context_free(cntx);
	return 0;
}

