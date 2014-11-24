#include "ruter/common.h"

inline static bool
is_sane(ruter_t *session, uint32_t id)
{
	return (session && id);
}

Place
*GetStop(ruter_t *session, uint32_t id)
{
	Place *stop = NULL;
	
	if (is_sane(session, id)) {
		char buf[32] = { '\0' };
		snprintf(buf, sizeof(buf), "%" PRIu32, id);
		
		if ((stop = GetPlaces(session, buf, NULL))) {
			if (RPT_STOP != stop->Type) {
				FreePlace(stop);
				stop = NULL;
			}
		}
	}
	
	return stop;
}
