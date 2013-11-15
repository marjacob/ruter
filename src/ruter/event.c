#include <stdlib.h>
#include <string.h>
#include "json.h"
#include "ruter/event.h"

static struct ruter_event event_zero = { 0 };

struct ruter_event *ruter_event_init(void)
{
	struct ruter_event *event = malloc(sizeof(*event));
	*event = event_zero;
	return event;
}

void ruter_event_free(struct ruter_event *event)
{
	if (NULL != event) {
		ruter_event_free(event->next);
		
		if (NULL != event->destination) {
			free(event->destination);
		}
		
		free(event);
	}
}

struct ruter_event *ruter_event_array_parse(json_value *data)
{
	if (NULL == data || json_array != data->type) {
		return NULL;
	}
	
	struct ruter_event *event = NULL;
	struct ruter_event *events = NULL;
	struct ruter_event *last_event = NULL;
	
	for (int i = 0, j = data->u.array.length; i < j; i++) {
		event = ruter_event_parse(data->u.array.values[i]);
		
		if (NULL == event) {
			continue;
		} else if (NULL == events) {
			events = event;
		} else {
			last_event->next = event;
		}
		
		last_event = event;
	}
	
	return events;
}

struct ruter_event *ruter_event_parse(json_value *data)
{
	if (NULL == data || json_object != data->type) {
		return NULL;
	}
	
	char *name = NULL;
	json_value *value = NULL;
	struct ruter_event *event = ruter_event_init();
	
	for (int i = 0, j = data->u.object.length; i < j; i++) {
		name = data->u.object.values[i].name;
		value = data->u.object.values[i].value;
		
		if (0 == strcmp("DestinationName", name)) {
			event->destination = strndup(
				value->u.string.ptr, 
				value->u.string.length);
		}
	}
	
	return event;
}
