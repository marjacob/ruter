#ifndef __H_RUTER_EVENT__
#define __H_RUTER_EVENT__

#include "json.h"
#include "ruter/constants.h"

struct ruter_event {
	char *destination;
	char *line_name;
	char *platform;
	int in_congestion;
	enum vehicle_mode vehicle_mode;
	struct ruter_event *next;
};

struct ruter_event *ruter_event_init(void);

void ruter_event_free(struct ruter_event *event);

struct ruter_event *ruter_event_array_parse(json_value *data);

struct ruter_event *ruter_event_parse(json_value *data);

#endif
