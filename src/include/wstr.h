#ifndef __H_WSTR__
#define __H_WSTR__

#include <stdlib.h>
#include <wchar.h>

typedef struct {
	size_t len;
	wchar_t *ptr;
} wstr_t;
 
inline static size_t
wstr_len(const wstr_t *s)
{
	return !s ? 0 : s->len;
}

inline static size_t
wstr_size(const wstr_t *s)
{
	return !s ? 0 : sizeof(*s->ptr) * (s->len + 1);
}

inline static wchar_t
*wstr_ptr(const wstr_t *s)
{
	return !s ? NULL : s->ptr;
}

inline static int
wstr_empty(const wstr_t *s)
{
	return !wstr_ptr(s) || !wstr_len(s);
}

inline static void
wstr_free(wstr_t *s)
{
	free(s);
}

wstr_t
*wstr_dup(const wstr_t *s);

wstr_t
*wstr_wcs(const wchar_t *s, size_t n);

wstr_t
*wstr_mbs(const char *s, size_t n);

size_t
wstr_len(const wstr_t *s);

wchar_t
*wstr_ptr(const wstr_t *s);

void
wstr_free(wstr_t *s);

#endif

