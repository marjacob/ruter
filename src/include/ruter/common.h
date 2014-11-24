#ifndef __H_RUTER_COMMON__
#define __H_RUTER_COMMON__

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "ruter/api.h"
#include "ruter/json.h"

wchar_t *wchar(const char *s, size_t n);

bool parse_area(Place *area, const json_value *json);

bool parse_poi(Place *poi, const json_value *json);

bool parse_stop(Place *stop, const json_value *json);

bool parse_street(Place *street, const json_value *json);

bool parse_place(Place *place, const json_value *json);

bool parse_places(Place *place, size_t n, const json_value *json);

inline static wchar_t *jsontowcs(const json_value *json)
{
	return (json && json_string == json->type)
		? wchar(json->u.string.ptr, json->u.string.length)
		: NULL;
}

#endif
