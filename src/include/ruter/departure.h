#ifndef __H_RUTER_EVENT__
#define __H_RUTER_EVENT__

#include "json.h"
#include "ruter/constants.h"

struct ruter_departure {
	char *destination;
	char *line_name;
	char *platform;
	int in_congestion;
	enum vehicle_mode vehicle_mode;
	struct ruter_departure *next;
};

struct ruter_departure
*ruter_departure_init(void);

void
ruter_departure_free(struct ruter_departure *event);

struct ruter_departure
*ruter_departure_parse(json_value *data);

#endif
