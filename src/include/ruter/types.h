#ifndef __H_RUTER_TYPES__
#define __H_RUTER_TYPES__

#include <stddef.h>

struct ruter_string {
	size_t length;
	wchar_t *ptr;
};

#endif
