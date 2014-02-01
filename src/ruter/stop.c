#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json.h"
#include "ruter/line.h"
#include "ruter/stop.h"
#include "ruter/util.h"

static struct ruter_stop stop_zero = { 0 };

static struct ruter_stop
*ruter_stop_init(void)
{
	struct ruter_stop *stop = malloc(sizeof(*stop));
	*stop = stop_zero;
	return stop;
}

static struct ruter_stop
*ruter_stop_array_parse(json_value *data)
{
	if (NULL == data || json_array != data->type) {
		return NULL;
	}

	struct ruter_stop *stop = NULL;
	struct ruter_stop *stops = NULL;
	struct ruter_stop *last_stop = NULL;

	for (int i = 0, j = data->u.array.length; i < j; i++) {
		stop = ruter_stop_parse(data->u.array.values[i]);

		if (NULL == stop) {
			continue;
		} else if (NULL == stops) {
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
	if (NULL != stop) {
		ruter_line_free(stop->lines);
		ruter_stop_free(stop->stops);
		ruter_stop_free(stop->next);
		ruter_safe_free(stop->name.ptr);
		ruter_safe_free(stop->district.ptr);
		ruter_safe_free(stop->zone.ptr);
		free(stop);
	}
}

struct ruter_stop
*ruter_stop_parse(json_value *data)
{
	if (NULL == data) {
		return NULL;
	} else if (json_array == data->type) {
		return ruter_stop_array_parse(data);
	} else if (json_object != data->type) {
		return NULL;
	}

	char *name = NULL;
	json_value *value = NULL;
	struct ruter_stop *stop = ruter_stop_init();

	for (int i = 0, j = data->u.object.length; i < j; i++) {
		name = data->u.object.values[i].name;
		value = data->u.object.values[i].value;

		if (0 == strcmp("ID", name)) {
			if (0 == (stop->id = value->u.integer)) {
				ruter_stop_free(stop);
				return NULL;
			}
		} else if (0 == strcmp("District", name)) {
			ruter_strfill(&stop->district, value);
		} else if (0 == strcmp("Name", name)) {
			ruter_strfill(&stop->name, value);
		} else if (0 == strcmp("Zone", name)) {
			ruter_strfill(&stop->zone, value);
		} else if (0 == strcmp("Type", name)) {
			stop->type = value->u.integer;
		} else if (0 == strcmp("Stops", name)) {
			stop->stops = ruter_stop_array_parse(value);
		} else if (0 == strcmp("Lines", name)) {
			stop->lines = ruter_line_parse(value);
		} else if (0 == strcmp("RealTimeStop", name)) {
			stop->realtime = value->u.boolean;
		}
	}

	return stop;
}
