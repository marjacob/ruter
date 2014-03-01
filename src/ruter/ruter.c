#include <curl/curl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "json.h"
#include "ruter/ruter.h"

struct ruter_stop
*ruter_find(ruter_t *session, char *place)
{
	int success = 0;
	char *name = NULL;
	json_value *data = NULL;
	struct ruter_stop *stops = NULL;

	if (!(name = curl_easy_escape(session->curl, place, 0))) {
		return NULL;
	} else {
		success = ruter_rest(session, "Place/FindPlaces", name);
		curl_free(name);
	}

	if (!success) {
		return NULL;
	}

	if (!(data = json_parse(session->buf, session->bufsize))) {
		return NULL;
	}

	stops = ruter_stop_parse(data);
	json_value_free(data);

	return stops;
}

struct ruter_departure
*ruter_departures(ruter_t *session, int64_t id)
{
	char stop_id[32];
	json_value *data = NULL;

	snprintf(stop_id, sizeof(stop_id), "%" PRIi64, id);

	if (!ruter_rest(session, "RealTime/GetAllDepartures", stop_id)) {
		return NULL;
	}

	if (!(data = json_parse(session->buf, session->bufsize))) {
		return NULL;
	}

	struct ruter_departure *deps = ruter_departure_parse(data);
	json_value_free(data);

	return deps;
}

struct ruter_travel
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
		time->tm_mon, 
		time->tm_year, 
		time->tm_hour, 
		time->tm_min,
		after ? "true" : "false",
		from_id,
		to_id);
		
	if (!ruter_rest(session, "Travel/GetTravelsByPlaces", buf)) {
		return NULL;
	}
	
	json_value *data = NULL;
	
	if (!(data = json_parse(session->buf, session->bufsize))) {
		return NULL;
	}
	
	struct ruter_travel *proposals = ruter_travel_parse(data);
	json_value_free(data);

	return proposals;
}

int
ruter_rest(ruter_t *session, char *method, char *args)
{
	session->bufsize = snprintf(
		session->buf,
		session->bufcap,
		"%s/%s/%s",
		session->uri,
		method,
		args);

	if (0 > session->bufsize) {
		return 0;
	}

	session->code = curl_easy_setopt(
		session->curl,
		CURLOPT_URL,
		session->buf);

	if (CURLE_OK != session->code) {
		return 0;
	}

	session->bufsize = 0;
	session->code = curl_easy_perform(session->curl);

	return !session->code;
}
