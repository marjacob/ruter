#ifndef __H_RUTER__
#define __H_RUTER__

#include <inttypes.h>
#include <time.h>
#include "constants.h"
#include "departure.h"
#include "line.h"
#include "session.h"
#include "stop.h"
#include "travel.h"

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
ruter_rest(struct ruter_session *session, char *method, char *args);

/**
 * ruter_find() - Searches for a place.
 *
 * @session:	Pointer to Ruter session structure.
 * @place:	Partial or full name of a place.
 *
 * This function retrieves a graph of places matching the given place name.
 *
 * Return: Pointer to graph of stops on success or NULL on failure.
 */
struct ruter_stop
*ruter_find(struct ruter_session *session, char *place);

/**
 * ruter_departures() - Retrieves departures from a stop.
 *
 * @session:	Pointer to Ruter session structure.
 * @id:		Unique stop ID.
 *
 * This function retrieves a list of departures from a given stop.
 *
 * Return: Pointer to list of departures on success or NULL on failure.
 */
struct ruter_departure
*ruter_departures(struct ruter_session *session, int64_t id);

/**
 * ruter_travel() - Computes possible travel plans.
 *
 * @session:	Pointer to Ruter session structure.
 * @time:	Desired departure time.
 * @after:	Specifies whether the given time should be interpreted as
 *		the departure or arrival time.
 * @to_id:	Unique stop ID for starting point.
 * @from_id:	Unique stop ID for destination.
 *
 * This function computes possible travel plans between two stops, arriving
 * or departing at a given time.
 *
 * Return: Pointer to list of travel plans on success or NULL on failure.
 */
struct ruter_travel
*ruter_travel(
	struct ruter_session *session, 
	struct tm *time, 
	int after,
	int64_t from_id, 
	int64_t to_id);

#endif
