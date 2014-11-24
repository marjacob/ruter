#include "ruter/common.h"

inline static bool
is_sane(ruter_t *session, uint32_t id)
{
	return (session && id);
}

Departure
*GetDepartures(ruter_t *session, uint32_t id, struct tm *time)
{
	if (!is_sane(session, id)) {
		return NULL;
	}
	
	return NULL;
}
