#include "ruter/common.h"

inline static bool
is_sane(ruter_t *session, const char *name)
{
	return (session && name);
}

inline static size_t
add_location(char *dest, size_t size, const UTM32 *location)
{
	return !dest ? 0 : !location ? 0 : snprintf(
		dest, 
		size, 
		"?location=(x=%" PRIu32 ",y=%" PRIu32 ")",
		location->X, 
		location->Y
	);
}

Place
*GetPlaces(ruter_t *session, const char *name, const UTM32 *location)
{
	if (!is_sane(session, name)) {
		return NULL;
	}
	
	char buf[2048] = { '\0' };
	char *ptr = buf;
	size_t size = sizeof(buf), len = 0;
	
	ptr += (len = snprintf(ptr, size, "%s", name));
	size -= len;
	ptr += (len = add_location(ptr, size, location));
	size -= len;
	
	if (!ruter_rest(session, "Place/GetPlaces", buf)) {
		return NULL;
	}
	
	json_value *data = json_parse(session->buf, session->bufsize);

	if (!data) {
		return NULL;
	}
	
	size_t count = data->u.array.length;
	Place *place = calloc(count, sizeof(*place));
	
	if (ENOMEM != errno && place) {
		if (!parse_places(place, count, data)) {
			FreePlace(place);
		}
	}	

	json_value_free(data);
	
	return place;
}
