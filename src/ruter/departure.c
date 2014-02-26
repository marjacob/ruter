#include <stdlib.h>
#include <string.h>
#include "json.h"
#include "ruter/constants.h"
#include "ruter/departure.h"
#include "ruter/time.h"
#include "ruter/util.h"
#include "wstr.h"

static struct ruter_departure departure_zero = { 0 };

static struct ruter_departure
*departure_init(void)
{
	struct ruter_departure *dep = malloc(sizeof(*dep));
	*dep = departure_zero;
	return dep;
}

static struct ruter_departure
*departure_array_parse(json_value *data)
{
	if (!is_json_array(data)) {
		return NULL;
	}

	struct ruter_departure
		*dep = NULL,
		*deps = NULL,
		*last_dep = NULL;

	for (int i = 0, j = data->u.array.length; i < j; i++) {
		dep = ruter_departure_parse(data->u.array.values[i]);

		if (!dep) {
			continue;
		} else if (!deps) {
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
		wstr_free(dep->dest);
		wstr_free(dep->line_name);
		wstr_free(dep->platform);
		free(dep);
	}
}

struct ruter_departure
*ruter_departure_parse(json_value *data)
{
	if (is_json_array(data)) {
		return departure_array_parse(data);
	} else if (!is_json_object(data)) {
		return NULL;
	}

	char *name = NULL;
	json_value *value = NULL;
	struct ruter_departure *dep = departure_init();

	for (int i = 0, j = data->u.object.length; i < j; i++) {
		name = data->u.object.values[i].name;
		value = data->u.object.values[i].value;

		if (!strcmp("DestinationName", name)) {
			dep->dest = wstr_from_json(value);
		} else if (!strcmp("PublishedLineName", name)) {
			dep->line_name = wstr_from_json(value);
		} else if (!strcmp("DeparturePlatformName", name)) {
			dep->platform = wstr_from_json(value);
		} else if (!strcmp("VehicleMode", name)) {
			dep->vehicle = value->u.integer;
		} else if (!strcmp("InCongestion", name)) {
			dep->congested = value->u.boolean;
		} else if (!strcmp("AimedArrivalTime", name)) {
			ruter_time_parse(&dep->a_arrive, value);
		} else if (!strcmp("AimedDepartureTime", name)) {
			ruter_time_parse(&dep->a_depart, value);
		} else if (!strcmp("ExpectedArrivalTime", name)) {
			ruter_time_parse(&dep->e_arrive, value);
		} else if (!strcmp("ExpectedDepartureTime", name)) {
			ruter_time_parse(&dep->e_depart, value);
		}
	}

	return dep;
}

