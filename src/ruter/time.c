#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "json.h"
#include "ruter/time.h"

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
	
	if (NULL == time || 0 != strncmp("/Date(", time, 6)) {
		return 0;
	}
	
	char *endptr = NULL;
	unsigned long long int ms = strtoull(time + 6, &endptr, 10);
	time_t seconds = (ms / 1000) + ruter_time_offset(endptr);
	
	memcpy(tm, gmtime(&seconds), sizeof(*tm));
	
	return 1;
}
