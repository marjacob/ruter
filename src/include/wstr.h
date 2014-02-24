#ifndef __H_WSTR__
#define __H_WSTR__

#include <stdlib.h>
#include <wchar.h>

typedef struct {
	size_t length;
	wchar_t *ptr;
} wstr_t;

wstr_t *wstr_new(const wchar_t *s, size_t n);

wstr_t *wstr_from_mbs(const char *s, size_t n);

size_t wstr_len(const wstr_t *s);

wchar_t *wstr_ptr(const wstr_t *s);

void wstr_free(wstr_t *s);

#endif

