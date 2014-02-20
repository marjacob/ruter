#ifndef __H_CSRC__
#define __H_CSRC__

#include <stdlib.h>

typedef struct {
	size_t cur_row;
	size_t cur_col;
	size_t argc;
	char **argv;
} csrc_t;

csrc_t *csrc_init(size_t argc, char *argv[]);

char csrc_next(csrc_t *src);

void csrc_rewind(csrc_t *src);

void csrc_free(csrc_t *src);

#endif
