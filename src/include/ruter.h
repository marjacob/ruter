#ifndef __H_RUTER__
#define __H_RUTER__

#include <curl/curl.h>
#include <inttypes.h>
#include <stdlib.h>
#include "ruter/constants.h"
#include "ruter/event.h"
#include "ruter/line.h"
#include "ruter/session.h"
#include "ruter/stop.h"

const char* ruter_strerror(CURLcode code);

int ruter_rest(struct ruter_session *session, char *method, char *args);

struct ruter_stop *ruter_find(struct ruter_session *session, char *place);

struct ruter_event *ruter_realtime(struct ruter_session *session, char *id);

int ruter_is_realtime(struct ruter_session *session, char *stop_id);

#endif

