#ifndef __H_RUTER_TIME__
#define __H_RUTER_TIME__

#include <time.h>
#include "json.h"

/**
 * ruter_time_parse() - Parses a JSON time representation into a struct tm.
 * @tm:		Pointer to time structure.
 * @data:	Pointer to JSON value.
 *
 * ruter_time_parse() parses time formatted as "/Date(1391295600000+0100)/"
 * which is "/Date(unix_time+/-offset)/" into a more useful time structure.
 *
 * Return: Returns zero on failure.
 */
int
ruter_time_parse(struct tm *tm, json_value *data);

#endif
