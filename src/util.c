#include <stdlib.h>
#include <string.h>
#include "util.h"

char *util_strdup(const char *s)
{
	size_t len = strlen(s);
	char *dup = malloc(len + 1);
	
	if (!dup) {
		return NULL;
	} else {
		dup[len] = '\0';
	}
	
	return memcpy(dup, s, len);

}

char *util_strndup(char const *s, size_t n)
{
	size_t len = util_strnlen(s, n);
	char *dup = malloc(len + 1);
	
	if (!dup) {
		return NULL;
	} else {
		dup[len] = '\0';
	}
	
	return memcpy(dup, s, len);
}

size_t util_strnlen(const char *s, size_t maxlen)
{
	register const char *p;
	for (p = s; *p && maxlen--; ++p);
	return(p - s);
}
