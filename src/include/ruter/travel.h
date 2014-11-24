#ifndef __H_RUTER_TRAVEL__
#define __H_RUTER_TRAVEL__

#include <time.h>
#include "json.h"
#include "types.h"

typedef struct proposal_t {
	struct tm arrival;
	struct tm departure;
	struct proposal_t *next;
} proposal_t;

void
ruter_travel_free(proposal_t *proposals);

proposal_t
*ruter_travel_parse(json_value *data);

#endif
