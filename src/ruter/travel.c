#include <stdlib.h>
#include <string.h>
#include "ruter/stop.h"
#include "ruter/travel.h"

static struct ruter_travel travel_zero = { { 0 } };
static struct ruter_stage stage_zero = { 0 };

static struct ruter_travel
*ruter_travel_init(void)
{
	struct ruter_travel *proposal = malloc(sizeof(*proposal));
	*proposal = travel_zero;
	return proposal;
}

static struct ruter_stage
*ruter_stage_init(void)
{
	struct ruter_stage *stage = malloc(sizeof(*stage));
	*stage = stage_zero;
	return stage;
}

static void
ruter_stage_free(struct ruter_stage *stage)
{
	if (NULL != stage) {
		ruter_stage_free(stage->next);
		ruter_stop_free(stage->arrival_stop);
		ruter_stop_free(stage->departure_stop);
		free(stage);
	}
}

void
ruter_travel_free(struct ruter_travel *travel)
{
	if (NULL != travel) {
		ruter_travel_free(travel->next);
		ruter_stage_free(travel->stages);
		free(travel);
	}
}

static struct ruter_travel
*ruter_proposal_parse(json_value *data)
{
	if (NULL == data || json_array != data->type) {
		return NULL;
	}
	
	//char *name = NULL;
	//json_value *value = NULL;
	struct ruter_travel *proposal = ruter_travel_init();
	
	/* Temporary measure to stop compiler whining. */
	free(ruter_stage_init());
	
	for (int i = 0, j = data->u.object.length; i < j; i++) {
		//name = data->u.object.values[i].name;
		//value = data->u.object.values[i].value;
		
		
	}
	
	return proposal;
}

struct ruter_travel
*ruter_travel_parse(json_value *data)
{
	if (NULL == data) {
		return NULL;
	} else if (json_object == data->type && 1 == data->u.object.length) {
		return ruter_travel_parse(data->u.object.values[0].value);
	}
	
	int valid = 0;
	char *name = NULL;
	json_value *value = NULL;
	
	for (int i = 0, j = data->u.object.length; i < j; i++) {
		name = data->u.object.values[i].name;
		value = data->u.object.values[i].value;

		if (0 == strcmp("TravelProposals", name)) {
			valid = 1;
			break;
		}
	}
	
	if (!valid) {
		return NULL;
	}
	
	struct ruter_travel *proposals = NULL;
	struct ruter_travel *proposal = NULL;
	struct ruter_travel *last = NULL;
	
	for (int i = 0, j = data->u.object.length; i < j; i++) {
		name = data->u.object.values[i].name;
		value = data->u.object.values[i].value;
		proposal = ruter_proposal_parse(value);
		
		if (NULL == proposals) {
			proposals = proposal;
		} else {
			last->next = proposal;
		}
		
		last = proposal;
	}
	
	return proposals;
}
