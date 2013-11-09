#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json.h"
#include "ruter.h"

static size_t ruter_write(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	struct ruter_session *session = (struct ruter_session*)userdata;
	size_t data_size = (size * nmemb);
	size_t total_size = data_size + session->bufsize;
	
	if (total_size + 1 > session->bufcap) {
		session->buf = realloc(session->buf, total_size + 1);
		
		if (NULL == session->buf) {
			session->bufcap = 0;
			return 0;
		}
		
		session->bufcap = total_size;
	}
	
	memcpy(session->buf + session->bufsize, ptr, data_size);
	session->bufsize += data_size;
	session->buf[session->bufsize] = '\0';
	
	return data_size;
}

int ruter_init(struct ruter_session *session, size_t bufcap)
{
	if (0 != (session->code = curl_global_init(CURL_GLOBAL_ALL))) {
		return 0;
	}
	
	if (NULL == (session->curl = curl_easy_init())) {
		curl_global_cleanup();
		return 0;
	}
	
	curl_easy_setopt(session->curl, CURLOPT_USERAGENT, RUTER_USER_AGENT);
	curl_easy_setopt(session->curl, CURLOPT_WRITEFUNCTION, ruter_write);
	curl_easy_setopt(session->curl, CURLOPT_WRITEDATA, session);
	
	bufcap = (0 >= bufcap) ? RUTER_BUFFER_SIZE : bufcap;
	session->buf = malloc(bufcap);
	session->bufcap = bufcap;
	
	strncpy(session->uri, RUTER_DEFAULT_API_URI, sizeof(session->uri));
	
	return 1;
}

void ruter_close(struct ruter_session *session)
{
	curl_easy_cleanup(session->curl);
	curl_global_cleanup();
	
	session->curl = NULL;
	session->code = 0;
	session->bufcap = 0;
	session->bufsize = 0;
	
	if (NULL != session->uri) {
		free(session->buf);
		session->buf = NULL;
	}
	
	return;
}

const char* ruter_strerror(CURLcode code)
{
	return curl_easy_strerror(code);
}

int ruter_is_realtime(struct ruter_session *session, char *stop_id)
{
	ruter_rest(session, "Place/IsRealTimeStop", stop_id);
	
	return (0 == strncmp(session->buf, "true", 4));
}

static int parse_stop(json_value *data, struct ruter_stop *stop)
{
	if (NULL == data || NULL == stop || json_object != data->type) {
		return 0;
	}
	
	char *name = NULL;
	json_value *value = NULL;
	
	for (int i = 0, j = data->u.object.length; i < j; i++) {
		name = data->u.object.values[i].name;
		value = data->u.object.values[i].value;
		
		if (0 == strcmp("ID", name)) {
			stop->id = value->u.integer;
		} else if (0 == strcmp("District", name)) {
			stop->district = strndup(
				value->u.string.ptr, 
				value->u.string.length);
		} else if (0 == strcmp("Name", name)) {
			stop->name = strndup(
				value->u.string.ptr, 
				value->u.string.length);
		} else if (0 == strcmp("Zone", name)) {
			stop->zone = strndup(
				value->u.string.ptr, 
				value->u.string.length);
		} else if (0 == strcmp("Type", name)) {
			stop->type = (enum place_type)value->u.integer;
		} else if (0 == strcmp("Stops", name)) {
			struct ruter_stop *curr_stop = NULL;
			struct ruter_stop *last_stop = NULL;
			json_value *object = NULL;
			
			for (int m = 0, n = value->u.array.length; m < n; m++) {
				object = value->u.array.values[m];
				curr_stop = calloc(1, sizeof(*curr_stop));
				curr_stop->next = NULL;
				
				if (!parse_stop(object, curr_stop)) {
					free(curr_stop);
				} else if (NULL == stop->stops) {
					stop->stops = curr_stop;
					last_stop = curr_stop;
				} else {
					last_stop->next = curr_stop;
					last_stop = curr_stop;
				}
			}
		}
	}
	
	return 1;
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
	} else if (json_array != data->type) {
		json_value_free(data);
		return NULL;
	}
	
	struct ruter_stop *stop = NULL;
	struct ruter_stop *stops = NULL;
	struct ruter_stop *last_stop = NULL;
	json_value *object = NULL;
	
	for (int i = 0, j = data->u.array.length; i < j; i++) {
		object = data->u.array.values[i];
		stop = calloc(1, sizeof(*stop));
		stop->next = NULL;
		
		if (!parse_stop(object, stop)) {
			free(stop);
		} else if (NULL == stops) {
			stops = stop;
			last_stop = stop;
		} else {
			last_stop->next = stop;
			last_stop = stop;
		}
	}
	
	json_value_free(data);
	
	return stops;
}

void ruter_stop_free(struct ruter_stop *stop)
{
	if (NULL == stop) {
		return;
	}
	
	ruter_stop_free(stop->stops);
	ruter_stop_free(stop->next);
	
	if (NULL != stop->name) {
		free(stop->name);
	}
	
	if (NULL != stop->district) {
		free(stop->district);
	}
	
	if (NULL != stop->zone) {
		free(stop->zone);
	}
	
	free(stop);
	
	return;
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
