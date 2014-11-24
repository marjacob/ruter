#ifndef __H_RUTER_SESSION__
#define __H_RUTER_SESSION__

#include <curl/curl.h>
#include <stddef.h>
#include "ruter/constants.h"

/**
 * ruter_t - Contains the session state.
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
 * written. A ruter_t should not be shared between different threads, as it 
 * uses libraries that are not thread safe.
 */
typedef struct {
	CURL *curl;
	CURLcode code;
	size_t bufcap;
	size_t bufsize;
	struct curl_slist *header;
	char *buf;
	char uri[RUTER_API_LENGTH];
	const char *(*strerror)(CURLcode);
} ruter_t;

/**
 * ruter_open() - Initialises a session structure.
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
ruter_open(ruter_t *session, size_t bufcap);

/**
 * ruter_rest() - Performs a REST call.
 *
 * @session:	Pointer to Ruter session structure.
 * @method:	Remote method to call.
 * @args:	Method arguments.
 *
 * A call to the specified method with the specified arguments is performed
 * and the received data is stored in session->buf.
 *
 * Return: Nonzero value on success.
 */
int
ruter_rest(ruter_t *session, char *method, char *args);

/**
 * ruter_close() - Closes all open handles associated with a session structure.
 *
 * @session:	Pointer to Ruter session structure.
 *
 * This function frees any allocated memory and cleans up any resources
 * associated with libcurl.
 */
void
ruter_close(ruter_t *session);

#endif

