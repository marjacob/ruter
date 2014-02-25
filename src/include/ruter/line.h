#ifndef __H_RUTER_LINE__
#define __H_RUTER_LINE__

#include <inttypes.h>
#include "constants.h"
#include "json.h"
#include "types.h"
#include "wstr.h"

/**
 * struct ruter_line - Describes a line.
 *
 * @id:		Unique stop ID.
 * @type:	Describes the kind of transportation available.
 * @next	Pointer to next line in the linked list.
 * @name	Name of the line.
 *
 * This structure describes the various properties of a line.
 */
struct ruter_line {
	int64_t id;
	enum transport_type type;
	struct ruter_line *next;
	wstr_t *name;
};

/**
 * ruter_line_free() - Frees a line and all its fields.
 *
 * @stop:	Pointer to line.
 *
 * Recursively frees a line and everything the line structure points to.
 * Fields are not zeroed and may be readable after calling this function.
 * Take care not to do that.
 */
void
ruter_line_free(struct ruter_line *line);

/**
 * ruter_line_parse() - Builds a line structure from JSON data.
 *
 * @data:	Pointer to JSON data.
 *
 * JSON data is extracted and placed inside more intuitive data structures.
 * Calling this function will often produce a graph, so remember to
 * check the next pointer and stops pointer.
 *
 * Return: Pointer to list of lines on success or NULL on failure.
 */
struct ruter_line
*ruter_line_parse(json_value *data);

#endif
