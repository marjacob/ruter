#ifndef __H_ARGS__
#define __H_ARGS__

#include <stdbool.h>
#include <stdlib.h>
#include "args/token.h"

typedef struct args {
	char *find;
	char *show;
} args_t;

args_t *args_parse(tok_t *tokens);

void args_free(args_t *args);

#endif
