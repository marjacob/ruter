ruter
=====

REST based client for the Ruter JSON API written in C.

API
---

Initializing a new API session.

	int ruter_init(struct ruter_session *session, size_t bufsize);

Closing an existing API session.

	void ruter_close(struct ruter_session *session);

Searching for available stops and other kinds of geographical locations.

	struct ruter_stop *ruter_find(struct ruter_session *session, char *place);

Finding departures at a location identified by its ID.

	struct ruter_departure *ruter_departures(struct ruter_session *session, int64_t id);

Example
-------

Creating a new API session.

	struct ruter_session session;
	
	if (!ruter_init(&session, 0)) {
		/* error */
	}
	
	/* Perform some API calls. */
	
	ruter_close(&session);

Searching for a stop.

	struct ruter_stop *stops = ruter_find(session, place);
	
	/* Recurse through the stops graph. */
	
	ruter_stop_free(stops);

Find all departures from a stop.

	/* Find the ID of a stop. */
	
	struct ruter_departure *departures = ruter_departures(session, stop->id);
	
	/* Recurse through the departures graph. */
	
	ruter_departure_free(departures);

Libraries
---------

*   [JSON parser](https://github.com/udp/json-parser/) by James McLaughlin et al.
