#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json.h"
#include "ruter.h"

int ruter_is_realtime(struct ruter_session *session, char *stop_id)
{
	ruter_rest(session, "Place/IsRealTimeStop", stop_id);
	
	return (0 == strncmp(session->buf, "true", 4));
}

struct ruter_stop *ruter_find(struct ruter_session *session, char *place)
{
	int success = 0;
	char *name = NULL;
	
	if (NULL == (name = curl_easy_escape(session->curl, place, 0))) {
		return NULL;
	} else {
		success = ruter_rest(session, "Place/FindPlaces", name);
		curl_free(name);
	}
	
	if (!success) {
		return NULL;
	}
	
	json_value *data = json_parse(session->buf, session->bufsize);
	
	if (NULL == data) {
		return NULL;
	}
	
	struct ruter_stop *stops = ruter_stop_array_parse(data);
	json_value_free(data);
	
	return stops;
}

struct ruter_event *ruter_realtime(struct ruter_session *session, char *id)
{	
	if (!ruter_rest(session, "RealTime/GetRealTimeData", id)) {
		return NULL;
	}
	
	json_value *data = json_parse(session->buf, session->bufsize);
	
	if (NULL == data) {
		return NULL;
	}
	
	struct ruter_event *events = ruter_event_array_parse(data);
	json_value_free(data);
	
	return events;
}

int ruter_rest(struct ruter_session *session, char *method, char *args)
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
