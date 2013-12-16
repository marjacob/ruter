#ifndef __H_RUTER_EVENT__
#define __H_RUTER_EVENT__

#include <time.h>
#include "constants.h"
#include "json.h"
#include "types.h"

struct ruter_departure {
	int in_congestion;
	enum vehicle_mode vehicle_mode;
	struct tm aimed_arrival;
	struct tm aimed_departure;
	struct tm expected_arrival;
	struct tm expected_departure;
	struct ruter_string destination;
	struct ruter_string line_name;
	struct ruter_string platform;
	struct ruter_departure *next;
};

void
ruter_departure_free(struct ruter_departure *event);

struct ruter_departure
*ruter_departure_parse(json_value *data);

#endif
