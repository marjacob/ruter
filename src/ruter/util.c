#include <stdlib.h>
#include <string.h>
#include "ruter/util.h"

void ruter_safe_free(void *ptr)
{
	if (NULL != ptr) {
		free(ptr);
	}
}

char *ruter_strndup(char const *s, size_t n)
{
	size_t len = ruter_strnlen(s, n);
	char *dup = malloc(len + 1);
	
	if (NULL == dup) {
		return NULL;
	}
	
	dup[len] = '\0';
	
	return memcpy(dup, s, len);
}

size_t ruter_strnlen(const char *s, size_t maxlen)
{
	register const char *e;
	size_t n;
	
	for (e = s, n = 0; '\0' != *e && n < maxlen; e++, n++);
	
	return n;
}
