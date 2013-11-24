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

Libraries
---------

*   The excellent [JSON parser](https://github.com/udp/json-parser/) by James McLaughlin et al.
