#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "json.h"
#include "ruter/types.h"
#include "ruter/util.h"

void ruter_safe_free(void *ptr)
{
	if (NULL != ptr) {
		free(ptr);
	}
}

void ruter_strfill(struct ruter_string *str, json_value *value)
{
	if (NULL == str) {
		return;
	} else if (NULL != value->u.string.ptr) {
		str->length = value->u.string.length;
		str->ptr = calloc(str->length + 1, sizeof(*str->ptr));
		str->length = mbstowcs(
			str->ptr, 
			value->u.string.ptr, 
			str->length);
		
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
