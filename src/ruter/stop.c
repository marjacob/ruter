#include <stdlib.h>
#include <string.h>
#include "json.h"
#include "ruter/line.h"
#include "ruter/stop.h"
#include "ruter/util.h"
#include "wstr.h"

static struct ruter_stop stop_zero = { 0 };

inline static struct ruter_stop
*ruter_stop_init(void)
{
	struct ruter_stop *stop = malloc(sizeof(*stop));
	*stop = stop_zero;
	return stop;
}

static struct ruter_stop
*ruter_stop_array_parse(const json_value *data)
{
	if (!is_json_array(data)) {
		return NULL;
	}

	struct ruter_stop 
		*stop = NULL, 
		*stops = NULL, 
		*last_stop = NULL;

	for (int i = 0, j = data->u.array.length; i < j; i++) {
		stop = ruter_stop_parse(data->u.array.values[i]);

		if (!stop) {
			continue;
		} else if (!stops) {
			stops = stop;
		} else {
			last_stop->next = stop;
		}

		last_stop = stop;
	}

	return stops;
}

void
ruter_stop_free(struct ruter_stop *stop)
{
	if (stop) {
		ruter_line_free(stop->lines);
		ruter_stop_free(stop->stops);
		ruter_stop_free(stop->next);
		wstr_free(stop->name);
		wstr_free(stop->district);
		wstr_free(stop->zone);
		free(stop);
	}
}

struct ruter_stop
*ruter_stop_parse(const json_value *data)
{
	if (is_json_array(data)) {
		return ruter_stop_array_parse(data);
	} else if (!is_json_object(data)) {
		return NULL;
	}

	char *name = NULL;
	json_value *value = NULL;
	struct ruter_stop *stop = ruter_stop_init();

	for (int i = 0, j = data->u.object.length; i < j; i++) {
		name = data->u.object.values[i].name;
		value = data->u.object.values[i].value;

		if (!strcmp("ID", name) && !(stop->id = value->u.integer)) {
			ruter_stop_free(stop);
			return NULL;
		} else if (!strcmp("District", name)) {
			stop->district = wstr_from_json(value);
		} else if (!strcmp("Name", name)) {
			stop->name = wstr_from_json(value);
		} else if (!strcmp("Zone", name)) {
			stop->zone = wstr_from_json(value);
		} else if (!strcmp("Type", name)) {
			stop->type = value->u.integer;
		} else if (!strcmp("Stops", name)) {
			stop->stops = ruter_stop_array_parse(value);
		} else if (!strcmp("Lines", name)) {
			stop->lines = ruter_line_parse(value);
		} else if (strcmp("RealTimeStop", name)) {
			stop->realtime = value->u.boolean;
		}
	}

	return stop;
}

