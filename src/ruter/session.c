#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include "ruter/constants.h"
#include "ruter/session.h"
#include "ruter/util.h"

static struct ruter_session session_zero = { 0 };

/**
 * write_data() - Internal libcurl callback function.
 *
 * @ptr:	Pointer to received data.
 * @size:	Unit size.
 * @nmemb:	Number of units.
 * @userdata:	Points to the Ruter session structure.
 *
 * This function gets called by libcurl as soon as there is data received
 * that needs to be saved. The size of the data pointed to by ptr is size 
 * multiplied with nmemb, it will not be zero terminated.
 *
 * Return: Describe the return value of foobar.
 */
static size_t
write_data(char *ptr, size_t size, size_t nmemb, void *userdata)
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

int
ruter_init(struct ruter_session *session, size_t bufcap)
{
	*session = session_zero;
	
	if (0 != (session->code = curl_global_init(CURL_GLOBAL_ALL))) {
		return 0;
	}

	if (NULL == (session->curl = curl_easy_init())) {
		curl_global_cleanup();
		return 0;
	}
	
	session->header = NULL;
	session->header = curl_slist_append(
		session->header, "Accept: application/json");
	session->header = curl_slist_append(
		session->header, "Accept-Charset: utf-8");

	curl_easy_setopt(session->curl, CURLOPT_HTTPHEADER, session->header);
	curl_easy_setopt(session->curl, CURLOPT_USERAGENT, RUTER_USER_AGENT);
	curl_easy_setopt(session->curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(session->curl, CURLOPT_WRITEDATA, session);

	session->bufcap = (0 >= bufcap) ? RUTER_BUFFER_SIZE : bufcap;
	if (NULL == (session->buf = malloc(session->bufcap))) {
		return 0;
	}
	
	strncpy(session->uri, RUTER_API_URI, RUTER_API_LENGTH);

	return 1;
}

void
ruter_close(struct ruter_session *session)
{
	if (NULL == session) {
		return;
	}
	
	curl_easy_cleanup(session->curl);
	curl_global_cleanup();
	curl_slist_free_all(session->header);
	ruter_safe_free(session->buf);
	
	*session = session_zero;

	return;
}
