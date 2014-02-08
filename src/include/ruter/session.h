#ifndef __H_RUTER_SESSION__
#define __H_RUTER_SESSION__

#include <curl/curl.h>
#include <stddef.h>

/**
 * struct ruter_session - Contains the session state.
 *
 * @curl:	Curl session handle.
 * 		Must not be shared between threads.
 * @code:	Curl error code.
 * @bufcap	Current buffer capacity.
 * @bufsize	Current amount of used buffer space.
 * @header	Curl HTTP header.
 * @buf		Pointer to buffer.
 * @uri		Zero terminated API uri.
 * @strerror	Pointer to function returning a string describing an error 
 * 		number.
 *
 * This is an internal structure and its fields are not supposed to be read or
 * written. A ruter_session should not be shared between different threads, as
 * it uses libraries that are not thread safe.
 */
struct ruter_session {
	CURL *curl;
	CURLcode code;
	size_t bufcap;
	size_t bufsize;
	struct curl_slist *header;
	char *buf;
	char uri[RUTER_API_LENGTH];
	const char *(*strerror)(CURLcode);
};

/**
 * ruter_init() - Initialises a session structure.
 *
 * @session:	Pointer to Ruter session structure.
 * @bufcap:	Initial maximum buffer size. This value maye increase.
 *
 * This function initialises a ruter session structure. The allocated buffer
 * will be enlarged automatically whenever necessary.
 *
 * Return: Returns nonzero on success.
 */
int
ruter_init(struct ruter_session *session, size_t bufcap);

/**
 * ruter_close() - Closes all open handles associated with a session structure.
 *
 * @session:	Pointer to Ruter session structure.
 *
 * This function frees any allocated memory and cleans up any resources
 * associated with libcurl.
 */
void
ruter_close(struct ruter_session *session);

#endif
