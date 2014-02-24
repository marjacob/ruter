#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "wstr.h"

wstr_t *wstr_new(const wchar_t *s, size_t n)
{
	if (!s) {
		return NULL;
	}
	
	n = n > 0 ? n : wcslen(s);
	wstr_t *str = malloc(sizeof(*str) + sizeof(*s) * (n + 1));
	str->ptr = (wchar_t*)(str + 1);
	wcsncpy(str->ptr, s, n);
	str->ptr[n] = '\0';
	str->length = n;

	return str;
}

wstr_t *wstr_mbs(const char *s, size_t n)
{
	if (!s) {
		return NULL;
	}
	
	n = n > 0 ? n : strlen(s);
	wstr_t *str = malloc(sizeof(*str) + sizeof(*s) * (n + 1));
	str->ptr = (wchar_t*)(str + 1);
	mbstate_t ps = { 0 };
	const char *src = s;

	if (0 > (str->length = mbsrtowcs(str->ptr, &src, str->length, &ps))) {
		free(str);
		str = NULL;
	} else {
		str->ptr[str->length] = L'\0';
	}

	return str;
}

size_t wstr_len(const wstr_t *s)
{
	return !s ? 0 : s->length;
}

wchar_t *wstr_ptr(const wstr_t *s)
{
	return !s ? NULL : s->ptr;
}

void wstr_free(wstr_t *s)
{
	free(s);
}

