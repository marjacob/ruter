#ifndef __H_RUTER_LINE__
#define __H_RUTER_LINE__

#include <inttypes.h>
#include "json.h"
#include "ruter/constants.h"
#include "ruter/types.h"

struct ruter_line {
	int64_t id;
	enum transport_type type;
	struct ruter_string name;
	struct ruter_line *next;
};

void
ruter_line_free(struct ruter_line *line);

struct ruter_line
*ruter_line_parse(json_value *data);

#endif
