#ifndef __H_RUTER_TRAVEL__
#define __H_RUTER_TRAVEL__

#include <time.h>
#include "json.h"
#include "stop.h"
#include "types.h"

struct ruter_travel_stage {
	struct ruter_stop *arrival_stop;
	struct ruter_stop *departure_stop;
	struct tm arrival_time;
	struct tm departure_time;
	struct ruter_travel_stage *next;
};

struct ruter_travel_proposal {
	struct tm arrival;
	struct tm departure;
	struct ruter_travel_stage *stages;
	struct ruter_travel_proposal *next;
};

void
ruter_travel_free(struct ruter_travel_proposal *travel);

struct ruter_travel_proposal
*ruter_travel_parse(json_value *data);

#endif
