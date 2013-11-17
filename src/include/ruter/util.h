#ifndef __H_RUTER_UTILITIES__
#define __H_RUTER_UTILITIES__

void ruter_safe_free(void *ptr);

char *ruter_strndup(char const *s, size_t n);

size_t ruter_strnlen(const char *s, size_t maxlen);

#endif
