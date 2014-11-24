#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include "ruter/constants.h"
#include "ruter/session.h"
#include "ruter/util.h"

static ruter_t session_zero = { 0 };

static size_t
write_data(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	ruter_t *session = (ruter_t*)userdata;
	size_t data_size = (size * nmemb);
	size_t total_size = data_size + session->bufsize;

	if (total_size + 1 > session->bufcap) {
		session->buf = realloc(session->buf, total_size + 1);
		session->bufcap = total_size;
	}

	memcpy(session->buf + session->bufsize, ptr, data_size);
	session->bufsize += data_size;
	session->buf[session->bufsize] = '\0';
	session->strerror = curl_easy_strerror;

	return data_size;
}

int
ruter_open(ruter_t *session, size_t bufcap)
{
	CURL *curl;
	struct curl_slist *hdr = NULL;
	*session = session_zero;
	
	if (0 != (session->code = curl_global_init(CURL_GLOBAL_ALL))) {
		return 0;
	}
	
	if (!(curl = curl_easy_init())) {
		curl_global_cleanup();
		return 0;
	}
	
	strncpy(session->uri, RUTER_API_URI, RUTER_API_LENGTH);
	session->bufcap = (0 >= bufcap) ? RUTER_BUFFER_SIZE : bufcap;
	session->buf = malloc(session->bufcap);	
	
	hdr = curl_slist_append(hdr, "Accept: application/json");
	hdr = curl_slist_append(hdr, "Accept-Charset: utf-8");
	
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, session->header);
	curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_SSLv3); 
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_USERAGENT, RUTER_USER_AGENT);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, session);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	
	session->curl = curl;
	session->header = hdr;

	return 1;
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
	
	char *url = NULL;
	if (!(url = curl_easy_escape(session->curl, session->buf, 0))) {
		return 0;
	}
	
	session->code = curl_easy_setopt(session->curl, CURLOPT_URL, url);

	if (CURLE_OK != session->code) {
		curl_free(url);
		return 0;
	}

	session->bufsize = 0;
	session->code = curl_easy_perform(session->curl);

	curl_free(url);
	return !session->code;
}

void
ruter_close(ruter_t *session)
{
	if (session) {
		curl_easy_cleanup(session->curl);
		curl_global_cleanup();
		curl_slist_free_all(session->header);
		free(session->buf);
		*session = session_zero;
	}
}

