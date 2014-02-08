#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>
#include "json.h"
#include "ruter/types.h"
#include "ruter/util.h"

int
is_json_array(const json_value *data)
{
	return (NULL != data && json_array == data->type);
}

int
is_json_object(const json_value *data)
{
	return (NULL != data && json_object == data->type)
}

void
ruter_strfill(struct ruter_string * const str, const json_value *const value)
{
	if (NULL == str) {
		return;
	} else if (NULL != value->u.string.ptr) {
		mbstate_t ps = { 0 };
		const char *src = value->u.string.ptr;
		
		str->length = value->u.string.length;
		str->ptr = calloc(str->length + 1, sizeof(*str->ptr));
		str->length = mbsrtowcs(str->ptr, &src, str->length, &ps);
		
		if (0 > str->length) {
			free(str->ptr);
			str->ptr = NULL;
			str->length = 0;
		} else {
			str->ptr[str->length] = L'\0';
		}
	} else {
		str->length = 0;
		str->ptr = NULL;
	}
}
