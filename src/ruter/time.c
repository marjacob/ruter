#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "json.h"
#include "ruter/time.h"

/**
 * ruter_time_offset() - Computes time offset in seconds.
 * @offset:	A character string representing a UTC offset formatted as 
 *		+hhmm or -hhmm.
 *
 * The ruter_time_offset() function takes a UTC offset and calculates its
 * representation in seconds. This is useful for computing the local time
 * of some unix time stamp.
 *
 * Return: Returns the UTC time offset in seconds.
 */
static int
ruter_time_offset(char *offset) {
	int h, m;
	
	if (NULL == offset || 5 > strnlen(offset, 5)) {
		return 0;
	} else {
		h = (offset[1] - 0x30) * 10 + (offset[2] - 0x30);
		m = (offset[3] - 0x30) * 10 + (offset[4] - 0x30);
	}
	
	return (offset[0] != '+' ? -1 : 1) * 60 * m + 60 * 60 * h;
}

int
ruter_time_parse(struct tm *tm, json_value *data)
{
	if (NULL == data || NULL == tm) {
		return 0;
	}
	
	char *time = data->u.string.ptr;
	char prefix[] = "/Date(";
	size_t prefixlen = sizeof(prefix) - 1;
	
	if (NULL == time || 0 != strncmp(prefix, time, prefixlen)) {
		return 0;
	}
	
	char *endptr = NULL;
	unsigned long long int ms = strtoull(time + prefixlen, &endptr, 10);
	time_t seconds = (ms / 1000) + ruter_time_offset(endptr);
	
	memcpy(tm, gmtime(&seconds), sizeof(*tm));
	
	return 1;
}
