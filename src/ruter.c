#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json.h"
#include "ruter.h"

struct ruter_stop
*ruter_find(struct ruter_session *session, char *place)
{
	int success = 0;
	char *name = NULL;
	json_value *data = NULL;
	struct ruter_stop *stops = NULL;

	if (NULL == (name = curl_easy_escape(session->curl, place, 0))) {
		return NULL;
	} else {
		success = ruter_rest(session, "Place/FindPlaces", name);
		curl_free(name);
	}

	if (!success) {
		return NULL;
	}

	if (NULL == (data = json_parse(session->buf, session->bufsize))) {
		return NULL;
	}

	stops = ruter_stop_parse(data);
	json_value_free(data);

	return stops;
}

struct ruter_departure
*ruter_departures(struct ruter_session *session, int64_t id)
{
	char stop_id[32];
	json_value *data = NULL;

	snprintf(stop_id, sizeof(stop_id), "%" PRIi64, id);

	if (!ruter_rest(session, "RealTime/GetAllDepartures", stop_id)) {
		return NULL;
	}

	if (NULL == (data = json_parse(session->buf, session->bufsize))) {
		return NULL;
	}

	struct ruter_departure *deps = ruter_departure_parse(data);
	json_value_free(data);

	return deps;
}

int
ruter_rest(struct ruter_session *session, char *method, char *args)
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

	return (0 == session->code);
}
