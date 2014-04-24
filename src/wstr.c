#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "wstr.h"

wstr_t
*wstr_dup(const wstr_t *s)
{
	if (!s) {
		return NULL;
	}
	
	size_t size = sizeof(*s) + wstr_size(s);
	wstr_t *dup = malloc(size);
	memcpy(dup, s, size);
	dup->ptr = (wchar_t *)(dup + 1);

	return dup;
}

wstr_t
*wstr_wcs(const wchar_t *s, size_t n)
{
	if (!s) {
		return NULL;
	}
	
	n = n > 0 ? n : wcslen(s);
	wstr_t *str = malloc(sizeof(*str) + sizeof(*str->ptr) * (n + 1));
	str->ptr = (wchar_t*)(str + 1);
	wcsncpy(str->ptr, s, n);
	str->ptr[n] = '\0';
	str->len = n;

	return str;
}

wstr_t
*wstr_mbs(const char *s, size_t n)
{
	if (!s) {
		return NULL;
	}
	
	n = n > 0 ? n : strlen(s);
	wstr_t *str = malloc(sizeof(*str) + sizeof(*str->ptr) * (n + 1));
	str->ptr = (wchar_t*)(str + 1);
	mbstate_t ps = { 0 };
	const char *src = s;
	
	if (0 > (str->len = mbsrtowcs(str->ptr, &src, n, &ps))) {
		free(str);
		str = NULL;
	} else {
		str->ptr[str->len] = L'\0';
	}

	return str;
}

