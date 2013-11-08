#ifndef __H_RUTER__
#define __H_RUTER__

#include <curl/curl.h>
#include <stdlib.h>

#define RUTER_BUFFER_SIZE 8192
#define RUTER_DEFAULT_API_URI "http://api.ruter.no/ReisRest"
#define RUTER_MAX_API_URI 256
#define RUTER_USER_AGENT "PosixRuterCLI/0.1-pre-alpha"

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

const char* ruter_strerror(CURLcode code);

int ruter_rest(struct ruter_session *session, char *method, char *args);

int ruter_find(struct ruter_session *session, char *place);

int ruter_is_realtime(struct ruter_session *session, char *stop_id);

#endif

