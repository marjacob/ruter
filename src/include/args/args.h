#ifndef __H_ARGS__
#define __H_ARGS__

#include <stdlib.h>
#include "args/token.h"

struct args {
	char *find;
	char *show;
	char *from;
	char *to;
};

int args_parse(struct args *args, size_t argc, char **argv);

void args_free(struct args *args);

#endif
