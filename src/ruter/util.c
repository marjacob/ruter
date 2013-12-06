#include <stdlib.h>
#include <string.h>
#include <wchar.h>
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
	
	if (0 >= len) {
		return NULL;
	}
	
	char *dup = malloc(len + 1);
	
	if (NULL == dup) {
		return NULL;
	}
	
	dup[len] = '\0';
	
	return memcpy(dup, s, len);
}

size_t ruter_strnlen(const char *s, size_t maxlen)
{
	if (NULL == s) {
		return 0;
	}
	
	register const char *e;
	size_t n;
	
	for (e = s, n = 0; '\0' != *e && n < maxlen; e++, n++);
	
	return n;
}
