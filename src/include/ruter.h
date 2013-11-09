#ifndef __H_RUTER__
#define __H_RUTER__

#include <curl/curl.h>
#include <inttypes.h>
#include <stdlib.h>

#ifndef RUTER_BUFFER_SIZE
#	define RUTER_BUFFER_SIZE 8192
#endif

#ifndef RUTER_DEFAULT_API_URI
#	define RUTER_DEFAULT_API_URI "http://api.ruter.no/ReisRest"
#endif

#ifndef RUTER_MAX_API_URI
#	define RUTER_MAX_API_URI 256
#endif

#ifndef RUTER_USER_AGENT
#	define RUTER_USER_AGENT "PosixRuterCLI/0.1-pre-alpha"
#endif

#ifndef RUTER_URI_SPACE
#	define RUTER_URI_SPACE "%20"
#endif

/* See http://labs.ruter.no/how-to-use-the-api/about-the-data-model.aspx */

enum place_type {
	PT_STOP = 0,
	PT_AREA = 1,
	PT_POI = 2,
	PT_STREET = 3
};

enum transportation_type {
	TT_WALKING = 0,
	TT_AIRPORT_BUS = 1,
	TT_BUS = 2,
	TT_DUMMY = 3,
	TT_AIRPORT_TRAIN = 4,
	TT_BOAT = 5,
	TT_TRAIN = 6,
	TT_TRAM = 7,
	TT_METRO = 8
};
 
enum vehicle_mode {
	VM_BUS = 0,
	VM_FERRY = 1,
	VM_RAIL = 2,
	VM_TRAM = 3,
	VM_METRO = 4
};

struct ruter_session {
	CURL *curl;
	CURLcode code;
	size_t bufcap;
	size_t bufsize;
	char *buf;
	char uri[RUTER_MAX_API_URI];
};

struct ruter_stop {
	int64_t id;
	char *name;
	char *district;
	char *zone;
	struct ruter_stop *next;
};

#ifndef RUTER_SPACE
#	define RUTER_SPACE(session) (session->bufcap - session->bufsize)
#endif

int ruter_init(struct ruter_session *session, size_t bufsize);

void ruter_close(struct ruter_session *session);

const char* ruter_strerror(CURLcode code);

int ruter_rest(struct ruter_session *session, char *method, char *args);

int ruter_find(struct ruter_session *session, char *place);

int ruter_is_realtime(struct ruter_session *session, char *stop_id);

#endif

