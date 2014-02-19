#include "csrc.h"

#include <stdlib.h>

csrc_t *csrc_init(size_t argc, char **argv)
{
	csrc_t *src = malloc(sizeof(*src));
	
	src->cur_row = 1;
	src->cur_col = 0;
	src->argc = argc;
	src->argv = argv;
	
	return src;
}

char csrc_next(csrc_t *src)
{
	char cur = '\0';
	
	if (src->argc <= src->cur_row) {
		return cur;
	}
	
	do {
		cur = src->argv[src->cur_row][src->cur_col++];
		
		if ('\0' == cur) {
			src->cur_row++;
			src->cur_col = 0;
		} else {
			break;
		}
		
	} while (src->cur_row < src->argc);
	
	return cur;
}

void csrc_reset(csrc_t *src)
{
	src->cur_row = 1;
	src->cur_col = 0;
}

void csrc_free(csrc_t *src)
{
	free(src);
}
