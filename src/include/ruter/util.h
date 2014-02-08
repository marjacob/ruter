#ifndef __H_RUTER_UTILITIES__
#define __H_RUTER_UTILITIES__

#include "json.h"
#include "types.h"

/**
 * ruter_strfill() - Extracts a string from JSON data.
 *
 * @str:	Pointer to destination string structure.
 * @value:	Pointer to JSON source value.
 *
 * Converts a JSON character array into a multibyte character array and copies
 * it into a string structure.
 */
void
ruter_strfill(struct ruter_string * const str, const json_value *const value);

#endif
