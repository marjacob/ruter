#ifndef __H_RUTER__
#define __H_RUTER__

#include <inttypes.h>
#include <time.h>
#include "constants.h"
#include "departure.h"
#include "line.h"
#include "session.h"
#include "stop.h"
#include "travel.h"

int
ruter_rest(struct ruter_session *session, char *method, char *args);

struct ruter_stop
*ruter_find(struct ruter_session *session, char *place);

struct ruter_departure
*ruter_departures(struct ruter_session *session, int64_t id);

struct ruter_travel
*ruter_travel(
	struct ruter_session *session, 
	struct tm *time, 
	int after,
	int64_t from_id, 
	int64_t to_id);

#endif
