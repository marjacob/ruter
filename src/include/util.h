#ifndef __H_UTILITIES__
#define __H_UTILITIES__

#include <stdlib.h>

char *util_strdup(const char *s);

char *util_strndup(char const *s, size_t n);

/**
 * util_strnlen() - Computes the length of a string.
 *
 * @s		Pointer to zero terminated string.
 * @maxlen	Maximum allowed string length.
 *
 * Computes the number of characters in the string pointed to by s, not 
 * including the termination character, but at most maxlen.
 *
 * Returns: The length of a string in characters.
 */
size_t util_strnlen(const char *s, size_t maxlen);

#endif
