#include <stdlib.h>
#include "ruter/travel.h"

static struct ruter_travel_proposal proposal_zero = { { 0 } };
static struct ruter_travel_stage stage_zero = { 0 };

static struct ruter_travel_proposal
*ruter_travel_proposal_init(void)
{
	struct ruter_travel_proposal *proposal = malloc(sizeof(*proposal));
	*proposal = proposal_zero;
	return proposal;
}

static struct ruter_travel_stage
*ruter_travel_stage_init(void)
{
	struct ruter_travel_stage *stage = malloc(sizeof(*stage));
	*stage = stage_zero;
	return stage;
}

void
ruter_travel_free(struct ruter_travel_proposal *travel)
{
	if (NULL != travel) {
		ruter_travel_free(travel->next);
		//ruter_travel_stage_free
		free(travel);
	}
}

struct ruter_travel_proposal
*ruter_travel_parse(json_value *data)
{
	struct ruter_travel_proposal *p = ruter_travel_proposal_init();
	struct ruter_travel_stage *s = ruter_travel_stage_init();
	free(p);
	free(s);
	return NULL;
}
