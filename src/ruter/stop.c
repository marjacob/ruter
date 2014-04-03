#include <stdlib.h>
#include <string.h>
#include "json.h"
#include "ruter/line.h"
#include "ruter/stop.h"
#include "ruter/util.h"
#include "wstr.h"

static stop_t stop_zero = { 0 };

inline static stop_t
*stop_init(void)
{
	stop_t *stop = malloc(sizeof(*stop));
	*stop = stop_zero;
	return stop;
}

static stop_t
*stop_array_parse(const json_value *data)
{
	if (!is_json_array(data)) {
		return NULL;
	}

	stop_t *curr = NULL;
	stop_t *head = NULL;
	stop_t *tail = NULL;

	for (int i = 0, j = data->u.array.length; i < j; i++) {
		curr = ruter_stop_parse(data->u.array.values[i]);

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
ruter_stop_free(stop_t *stop)
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

vehicle_t
ruter_stop_mode(const stop_t *stop)
{
	if (stop && PT_STOP == stop->type && wstr_len(stop->name)) {
		wchar_t *name = wstr_ptr(stop->name);

		if (wcsstr(name, L"tog]")) {
			return VM_RAIL;
		} else if (wcsstr(name, L"[buss]")) {
			return VM_BUS;
		} else if (wcsstr(name, L"[T-bane]")) {
			return VM_METRO;
		} else if (wcsstr(name, L"trikk")) {
			return VM_TRAM;
		} else if (wcsstr(name, L"[båt]")) {
			return VM_FERRY;
		}
	}

	return VM_NONE;
}

stop_t
*ruter_stop_parse(const json_value *data)
{
	if (is_json_array(data)) {
		return stop_array_parse(data);
	} else if (!is_json_object(data)) {
		return NULL;
	}

	char *name = NULL;
	json_value *value = NULL;
	stop_t *stop = stop_init();

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
			stop->stops = stop_array_parse(value);
		} else if (!strcmp("Lines", name)) {
			stop->lines = ruter_line_parse(value);
		} else if (!strcmp("RealTimeStop", name)) {
			stop->realtime = value->u.boolean;
		}
	}

	return stop;
}

