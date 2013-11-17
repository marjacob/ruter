#include <stdlib.h>
#include "ruter/util.h"

void ruter_safe_free(void *ptr)
{
	if (NULL != ptr) {
		free(ptr);
	}
}
