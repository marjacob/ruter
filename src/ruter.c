#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json.h"
#include "ruter.h"

static struct ruter_stop *stop_array_parse(json_value *data);
static struct ruter_stop *stop_parse(json_value *data);
static struct ruter_line *line_array_parse(json_value *data);
static struct ruter_line *line_parse(json_value *data);
static size_t write_data(char *ptr, size_t size, size_t nmemb, void *userdata);

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
	curl_easy_setopt(session->curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(session->curl, CURLOPT_WRITEDATA, session);
	
	session->bufcap = (0 >= bufcap) ? RUTER_BUFFER_SIZE : bufcap;
	session->buf = malloc(session->bufcap);
	
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
	
	struct ruter_stop *stops = stop_array_parse(data);
	json_value_free(data);
	
	return stops;
}

void ruter_stop_free(struct ruter_stop *stop)
{
	if (NULL != stop) {
		ruter_line_free(stop->lines);
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
	}
}

void ruter_line_free(struct ruter_line *line)
{
	if (NULL != line) {
		ruter_line_free(line->next);
		
		if (NULL != line->name) {
			free(line->name);
		}
		
		free(line);
	}
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

static struct ruter_stop *stop_parse(json_value *data)
{
	if (NULL == data || json_object != data->type) {
		return NULL;
	}
	
	char *name = NULL;
	json_value *value = NULL;
	struct ruter_stop *stop = calloc(1, sizeof(*stop));
	
	for (int i = 0, j = data->u.object.length; i < j; i++) {
		name = data->u.object.values[i].name;
		value = data->u.object.values[i].value;
		
		if (0 == strcmp("ID", name)) {
			if (0 == (stop->id = value->u.integer)) {
				ruter_stop_free(stop);
				return NULL;
			}
		} else if (0 == strcmp("District", name)) {
			stop->district = strndup(
				value->u.string.ptr, 
				value->u.string.length);
		} else if (0 == strcmp("Name", name)) {
			stop->name = strndup(
				value->u.string.ptr, 
				value->u.string.length);
		} else if (0 == strcmp("Zone", name)) {
			if (0 < value->u.string.length) {
				stop->zone = strndup(
					value->u.string.ptr, 
					value->u.string.length);
			} else {
				stop->zone = NULL;
			}
		} else if (0 == strcmp("Type", name)) {
			stop->type = (enum place_type)value->u.integer;
		} else if (0 == strcmp("Stops", name)) {
			stop->stops = stop_array_parse(value);
		} else if (0 == strcmp("Lines", name)) {
			stop->lines = line_array_parse(value);
		}
	}
	
	stop->next = NULL;
	
	return stop;
} 

static struct ruter_stop *stop_array_parse(json_value *data)
{
	if (NULL == data || json_array != data->type) {
		return NULL;
	}
	
	struct ruter_stop *stop = NULL;
	struct ruter_stop *stops = NULL;
	struct ruter_stop *last_stop = NULL;
	
	for (int i = 0, j = data->u.array.length; i < j; i++) {
		stop = stop_parse(data->u.array.values[i]);
		
		if (NULL == stop) {
			continue;
		} else if (NULL == stops) {
			stops = stop;
		} else {
			last_stop->next = stop;
		}
		
		last_stop = stop;
	}
	
	return stops;
}

static struct ruter_line *line_array_parse(json_value *data)
{
	if (NULL == data || json_array != data->type) {
		return NULL;
	}
	
	struct ruter_line *line = NULL;
	struct ruter_line *lines = NULL;
	struct ruter_line *last_line = NULL;
	
	for (int i = 0, j = data->u.array.length; i < j; i++) {
		line = line_parse(data->u.array.values[i]);
		
		if (NULL == line) {
			continue;
		} else if (NULL == lines) {
			lines = line;
		} else {
			last_line->next = line;
		}
		
		last_line = line;
	}
	
	return lines;
}

static struct ruter_line *line_parse(json_value *data)
{
	if (NULL == data || json_object != data->type) {
		return NULL;
	}
	
	char *name = NULL;
	json_value *value = NULL;
	struct ruter_line *line = calloc(1, sizeof(*line));
	
	for (int i = 0, j = data->u.object.length; i < j; i++) {
		name = data->u.object.values[i].name;
		value = data->u.object.values[i].value;
		
		if (0 == strcmp("LineID", name)) {
			if (0 == (line->id = value->u.integer)) {
				ruter_line_free(line);
				return NULL;
			}
		} else if (0 == strcmp("LineName", name)) {
			line->name = strndup(
				value->u.string.ptr, 
				value->u.string.length);
		} else if (0 == strcmp("Transportation", name)) {
			line->type = (enum transport_type)value->u.integer;
		}
	}
	
	line->next = NULL;
	
	return line;
}

static size_t write_data(char *ptr, size_t size, size_t nmemb, void *userdata)
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
