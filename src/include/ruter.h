#ifndef __H_RUTER__
#define __H_RUTER__

#include "ruter/constants.h"
#include "ruter/event.h"
#include "ruter/line.h"
#include "ruter/session.h"
#include "ruter/stop.h"

int ruter_rest(struct ruter_session *session, char *method, char *args);

struct ruter_stop *ruter_find(struct ruter_session *session, char *place);

struct ruter_event *ruter_realtime(struct ruter_session *session, int64_t id);

int ruter_is_realtime(struct ruter_session *session, int64_t id);

#endif
