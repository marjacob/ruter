#ifndef __H_RUTER_TIME__
#define __H_RUTER_TIME__

#include <time.h>
#include "json.h"

int
ruter_time_parse(struct tm *tm, json_value *data);

#endif
