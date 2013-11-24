#ifndef __H_RUTER__
#define __H_RUTER__

#include "ruter/constants.h"
#include "ruter/departure.h"
#include "ruter/line.h"
#include "ruter/session.h"
#include "ruter/stop.h"

int
ruter_rest(struct ruter_session *session, char *method, char *args);

struct ruter_stop
*ruter_find(struct ruter_session *session, char *place);

struct ruter_departure
*ruter_departures(struct ruter_session *session, int64_t id);

#endif
