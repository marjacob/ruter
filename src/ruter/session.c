#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include "ruter/constants.h"
#include "ruter/session.h"

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
	session->strerror = curl_easy_strerror;
	
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
	curl_easy_setopt(session->curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(session->curl, CURLOPT_WRITEDATA, session);
	
	session->bufcap = (0 >= bufcap) ? RUTER_BUFFER_SIZE : bufcap;
	if (NULL == (session->buf = malloc(session->bufcap))) {
		return 0;
	}
	
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