#ifndef __H_MAIN__
#define __H_MAIN__

#include "ruter/constants.h"

typedef struct {
	char *from;
	char *search;
	char *to;
	size_t limit;
	vehicle_t kind; 
} args_t;

#endif

