#ifndef __H_RUTER_STOP__
#define __H_RUTER_STOP__

#include <inttypes.h>
#include "json.h"
#include "ruter/constants.h"
#include "ruter/line.h"
#include "ruter/types.h"

struct ruter_stop {
	int64_t id;
	enum place_type type;
	int realtime;
	struct ruter_string name;
	struct ruter_string district;
	struct ruter_string zone;
	struct ruter_line *lines;
	struct ruter_stop *stops;
	struct ruter_stop *next;
};

struct ruter_stop
*ruter_stop_init(void);

void
ruter_stop_free(struct ruter_stop *stop);

struct ruter_stop
*ruter_stop_parse(json_value *data);

#endif
