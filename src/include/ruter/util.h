#ifndef __H_RUTER_UTILITIES__
#define __H_RUTER_UTILITIES__

#include <stdlib.h>
#include "json.h"
#include "types.h"
#include "wstr.h"

inline static wstr_t *wstr_from_json(const json_value *v)
{
	return v ? wstr_mbs(v->u.string.ptr, v->u.string.length) : NULL;
}

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
is_json_object(const json_value *data);

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
ruter_strfill(struct ruter_string *str, const json_value *value);

#endif

