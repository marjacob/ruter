#ifndef __H_ARGS__
#define __H_ARGS__

#include <stdbool.h>
#include <stdlib.h>
#include "args/token.h"

typedef struct args {
	char *find;
	char *show;
	char *from;
	char *to;
} args_t;

args_t *args_parse(size_t argc, char **argv);

void args_free(args_t *args);

#endif
