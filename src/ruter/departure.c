#include <stdlib.h>
#include <string.h>
#include "json.h"
#include "ruter/constants.h"
#include "ruter/departure.h"
#include "ruter/time.h"
#include "ruter/util.h"

static struct ruter_departure departure_zero = { 0 };

static struct ruter_departure
*ruter_departure_init(void)
{
	struct ruter_departure *dep = malloc(sizeof(*dep));
	*dep = departure_zero;
	return dep;
}

static struct ruter_departure
*ruter_departure_array_parse(json_value *data)
{
	if (NULL == data || json_array != data->type) {
		return NULL;
	}

	struct ruter_departure
		*dep = NULL,
		*deps = NULL,
		*last_dep = NULL;

	for (int i = 0, j = data->u.array.length; i < j; i++) {
		dep = ruter_departure_parse(data->u.array.values[i]);

		if (NULL == dep) {
			continue;
		} else if (NULL == deps) {
			deps = dep;
		} else {
			last_dep->next = dep;
		}

		last_dep = dep;
	}

	return deps;
}

void
ruter_departure_free(struct ruter_departure *dep)
{
	if (NULL != dep) {
		ruter_departure_free(dep->next);
		free(dep->dest.ptr);
		free(dep->line_name.ptr);
		free(dep->platform.ptr);
		free(dep);
	}
}

struct ruter_departure
*ruter_departure_parse(json_value *data)
{
	if (NULL == data) {
		return NULL;
	} else if (json_array == data->type) {
		return ruter_departure_array_parse(data);
	} else if (json_object != data->type) {
		return NULL;
	}

	char *name = NULL;
	json_value *value = NULL;
	struct ruter_departure *dep = ruter_departure_init();

	for (int i = 0, j = data->u.object.length; i < j; i++) {
		name = data->u.object.values[i].name;
		value = data->u.object.values[i].value;

		if (0 == strcmp("DestinationName", name)) {
			ruter_strfill(&dep->dest, value);
		} else if (0 == strcmp("PublishedLineName", name)) {
			ruter_strfill(&dep->line_name, value);
		} else if (0 == strcmp("DeparturePlatformName", name)) {
			ruter_strfill(&dep->platform, value);
		} else if (0 == strcmp("VehicleMode", name)) {
			dep->vehicle = value->u.integer;
		} else if (0 == strcmp("InCongestion", name)) {
			dep->congested = value->u.boolean;
		} else if (0 == strcmp("AimedArrivalTime", name)) {
			ruter_time_parse(&dep->a_arrival, value);
		} else if (0 == strcmp("AimedDepartureTime", name)) {
			ruter_time_parse(&dep->a_depart, value);
		} else if (0 == strcmp("ExpectedArrivalTime", name)) {
			ruter_time_parse(&dep->e_arrival, value);
		} else if (0 == strcmp("ExpectedDepartureTime", name)) {
			ruter_time_parse(&dep->e_depart, value);
		}
	}

	return dep;
}
