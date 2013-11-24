ruter
=====

REST based client for the Ruter JSON API written in C.

API
---

Searching for available stops and other kinds of geographical locations.

	struct ruter_stop *ruter_find(struct ruter_session *session, char *place);

Finding departures at a location identified by its ID.

	struct ruter_departure *ruter_departures(struct ruter_session *session, int64_t id);

Libraries
---------

*   The excellent [JSON parser](https://github.com/udp/json-parser/) by James McLaughlin et al.
