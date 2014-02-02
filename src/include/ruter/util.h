#ifndef __H_RUTER_UTILITIES__
#define __H_RUTER_UTILITIES__

#include "json.h"
#include "types.h"

/**
 * ruter_safe_free() - Frees dynamically allocated memory and ignores NULL.
 * @ptr:	Pointer to dynamically allocated memory.
 *
 * Frees dynamically allocated memory by calling free(). The only difference
 * is that this function checks for NULL pointers and silently returns
 * pretending that nothing happened.
 */
void
ruter_safe_free(void *const ptr);

/**
 * ruter_strfill() - Extracts a string from JSON data.
 * @str:	Pointer to destination string structure.
 * @value:	Pointer to JSON source value.
 *
 * Converts a JSON character array into a multibyte character array and copies
 * it into a string structure.
 */
void
ruter_strfill(struct ruter_string * const str, const json_value *const value);

#endif
