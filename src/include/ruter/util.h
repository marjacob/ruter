#ifndef __H_RUTER_UTILITIES__
#define __H_RUTER_UTILITIES__

#include "json.h"
#include "types.h"

/**
 * is_json_array() - Checks if a JSON value is an array.
 *
 * @data:	Pointer to JSON value.
 *
 * Checks whether a JSON value pointer points to a JSON array.
 *
 * Return: Returns nonzero if the value is a valid JSON array.
 */
int
is_json_array(const json_value *data);

/**
 * is_json_object() - Checks if a JSON value is an object.
 *
 * @data:	Pointer to JSON value.
 *
 * Checks whether a JSON value pointer points to a JSON object.
 *
 * Return: Returns nonzero if the value is a valid JSON object.
 */
int
is_json_object(const json_value *data)

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
