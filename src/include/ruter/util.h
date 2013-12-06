#ifndef __H_RUTER_UTILITIES__
#define __H_RUTER_UTILITIES__

#include "json.h"
#include "ruter/types.h"

void ruter_safe_free(void *ptr);

void ruter_strfill(struct ruter_string *str, json_value *value);

#endif
