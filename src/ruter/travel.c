#include <stdlib.h>
#include <string.h>
#include "ruter/stop.h"
#include "ruter/time.h"
#include "ruter/travel.h"
#include "ruter/util.h"

static proposal_t proposal_zero = { { 0 } };

static proposal_t
*ruter_travel_init(void)
{
	proposal_t *proposal = malloc(sizeof(*proposal));
	*proposal = proposal_zero;
	return proposal;
}

void
ruter_travel_free(proposal_t *proposals)
{
	if (proposals) {
		ruter_travel_free(proposals->next);
		free(proposals);
	}
}

proposal_t
*parse_proposal(json_value *data)
{
	if (!(data && json_object == data->type)) {
		return NULL;
	}
	
	proposal_t *proposal = ruter_travel_init();
	
	char *name = NULL;
	json_value *value = NULL;

	for (size_t i = 0, j = data->u.object.length; i < j; i++) {
		name = data->u.object.values[i].name;
		value = data->u.object.values[i].value;
	
		if (!strcmp("ArrivalTime", name)) {
			ruter_time_parse(&proposal->arrival, value);
		} else if (!strcmp("DepartureTime", name)) {
			ruter_time_parse(&proposal->departure, value);
		}
	}
	
	return proposal;
}

inline static json_value
*get_proposals(json_value *data)
{
	static const char entry[] = "TravelProposals";
	
	if (!(data && json_object == data->type)) {
		return NULL;
	}
	
	char *name = NULL;
	json_value *value = NULL;

	for (size_t i = 0, j = data->u.object.length; i < j; i++) {
		name = data->u.object.values[i].name;
		value = data->u.object.values[i].value;
		
		/* Return JSON array of travel proposals. */
		if (!strncmp(entry, name, sizeof(entry) - 1)) {
			return value;
		}
	}
	
	return NULL;
}

proposal_t
*ruter_travel_parse(json_value *data)
{
	if (NULL == data) {
		return NULL;
	} else if (json_object == data->type && 1 == data->u.object.length) {
		return ruter_travel_parse(data->u.object.values[0].value);
	}
	
	json_value *json = get_proposals(data);
	
	if (!json) {
		return NULL;
	}
	
	proposal_t *curr = NULL;
	proposal_t *head = NULL;
	proposal_t *tail = NULL;
	
	for (int i = 0, j = json->u.array.length; i < j; i++) {
		curr = parse_proposal(json->u.array.values[i]);
		
		if (!head) {
			head = curr;
		} else {
			tail->next = curr;
		}
		
		tail = curr;
	}

	return head;
}

