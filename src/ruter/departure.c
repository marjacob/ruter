#include <stdlib.h>
#include <string.h>
#include "ruter/constants.h"
#include "ruter/departure.h"
#include "ruter/json.h"
#include "ruter/time.h"
#include "ruter/util.h"
#include "wstr.h"

static departure_t departure_zero = { 0 };

inline static departure_t
*departure_init(void)
{
	departure_t *dep = malloc(sizeof(*dep));
	*dep = departure_zero;
	return dep;
}

static departure_t
*departure_array_parse(json_value *data)
{
	if (!is_json_array(data)) {
		return NULL;
	}

	departure_t *curr = NULL;
	departure_t *head = NULL;
	departure_t *tail = NULL;

	for (int i = 0, j = data->u.array.length; i < j; i++) {
		curr = ruter_departure_parse(data->u.array.values[i]);

		if (!curr) {
			continue;
		} else if (!head) {
			head = curr;
		} else {
			tail->next = curr;
		}

		tail = curr;
	}
	
	return head;
}

void
ruter_departure_free(departure_t *dep)
{
	if (dep) {
		ruter_departure_free(dep->next);
		wstr_free(dep->dest);
		wstr_free(dep->line_name);
		wstr_free(dep->platform);
		free(dep);
	}
}

departure_t
*ruter_departure_parse(json_value *data)
{
	if (is_json_array(data)) {
		return departure_array_parse(data);
	} else if (!is_json_object(data)) {
		return NULL;
	}

	char *name = NULL;
	json_value *value = NULL;
	departure_t *dep = departure_init();

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

