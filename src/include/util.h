#ifndef __H_UTILITIES__
#define __H_UTILITIES__

#include <stdlib.h>

/**
 * util_max() - Selects the larger of two values.
 *
 * @x		An unsigned integer.
 * @y		An unsigned integer.
 *
 * Compares two unsigned integers and returns the largest of them.
 *
 * Returns: Largest value.
 */
inline static size_t util_max(size_t x, size_t y)
{
	return x > y ? x : y;
}

/**
 * util_min() - Selects the smaller of two values.
 *
 * @x		An unsigned integer.
 * @y		An unsigned integer.
 *
 * Compares two unsigned integers and returns the smallest of them.
 *
 * Returns: Smallest value.
 */
inline static size_t util_min(size_t x, size_t y)
{
	return x < y ? x : y;
}

/**
 * util_strdup() - Creates a duplicate of a string.
 *
 * @s		Pointer to zero terminated string.
 *
 * Duplicates a zero terminated string.
 *
 * Returns: Pointer to duplicate string.
 */
char *util_strdup(const char *s);

/**
 * util_strndup() - Creates a duplicate of a string.
 *
 * @s		Pointer to zero terminated string.
 * @maxlen	Maximum allowed string length.
 *
 * Duplicates at most n characters from the source string.
 *
 * Returns: Pointer to duplicate string.
 */
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
