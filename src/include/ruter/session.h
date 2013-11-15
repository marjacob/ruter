#ifndef __H_RUTER_SESSION__
#define __H_RUTER_SESSION__

#include <curl/curl.h>
#include <stddef.h>

struct ruter_session {
	CURL *curl;
	CURLcode code;
	size_t bufcap;
	size_t bufsize;
	char *buf;
	char uri[RUTER_MAX_API_URI];
};

int ruter_init(struct ruter_session *session, size_t bufsize);

void ruter_close(struct ruter_session *session);

#endif
