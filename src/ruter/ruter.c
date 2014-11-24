#include <curl/curl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ruter/json.h"
#include "ruter/ruter.h"

stop_t
*ruter_find(ruter_t *session, char *place)
{
	int success = 0;
	char *name = NULL;

	if (!(name = curl_easy_escape(session->curl, place, 0))) {
		return NULL;
	} else {
		success = ruter_rest(session, "ReisRest/Place/FindPlaces", name);
		curl_free(name);
	}

	if (!success) {
		return NULL;
	}
	
	json_value *data = NULL;

	if (!(data = json_parse(session->buf, session->bufsize))) {
		return NULL;
	}

	stop_t *stops = ruter_stop_parse(data);
	json_value_free(data);

	return stops;
}

stop_t
*ruter_guess(ruter_t *session, char *place, vehicle_t mode)
{
	stop_t *match = NULL;
	stop_t *head = ruter_find(session, place);
	
	if (!head) {
		return NULL;
	}
	
	for (stop_t *stop = head; stop->next; stop = stop->next) {
		if (ruter_stop_match(stop, mode)) {
			match = ruter_stop_copy(stop);
			break;
		}
	}

	ruter_stop_free(head);
	return match;
}

departure_t
*ruter_departures(ruter_t *session, int64_t id)
{
	char stop_id[32];
	json_value *data = NULL;

	snprintf(stop_id, sizeof(stop_id), "%" PRIi64, id);

	if (!ruter_rest(session, "ReisRest/RealTime/GetAllDepartures", stop_id)) {
		return NULL;
	}

	if (!(data = json_parse(session->buf, session->bufsize))) {
		return NULL;
	}

	departure_t *deps = ruter_departure_parse(data);
	json_value_free(data);

	return deps;
}

proposal_t
*ruter_travel(
	ruter_t *session, 
	struct tm *time,
	int after, 
	int64_t from_id, 
	int64_t to_id)
{
	char buf[512];
	
	snprintf(
		buf, 
		sizeof(buf), 
		"?time=%02d%02d%04d%02d%02d"
		"&isAfter=%s"
		"&fromplace=%" PRIi64
		"&toplace=%" PRIi64,
		time->tm_mday, 
		time->tm_mon + 1, 
		time->tm_year + 1900, 
		time->tm_hour, 
		time->tm_min,
		after ? "true" : "false",
		from_id,
		to_id);
	
	if (!ruter_rest(session, "ReisRest/Travel/GetTravelsByPlaces", buf)) {
		return NULL;
	}
	
	json_value *data = NULL;
	
	if (!(data = json_parse(session->buf, session->bufsize))) {
		return NULL;
	}
	
	proposal_t *proposals = ruter_travel_parse(data);
	json_value_free(data);

	return proposals;
}
