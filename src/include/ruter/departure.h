#ifndef __H_RUTER_EVENT__
#define __H_RUTER_EVENT__

#include <time.h>
#include "constants.h"
#include "json.h"
#include "types.h"

/**
 * struct ruter_departure - Describes a departure.
 *
 * @congested:	Indicates whether a departure is affected by congestion.
 * @vehicle:	Describes the kind of stop this is.
 * @a_arrival:	Aimed arrival time.
 * @a_depart:	Aimed departure time.
 * @e_arrival:	Expected arrival time.
 * @e_depart:	Expected departure time.
 * @dest:	Name of the destination.
 * @line_name:	Name of the line.
 * @platform	Departure platform.
 * @next	Next departure in the linked list.
 *
 * This structure describes the various properties of a departure.
 */
struct ruter_departure {
	int congested;
	enum vehicle_mode vehicle;
	struct tm a_arrival;
	struct tm a_depart;
	struct tm e_arrival;
	struct tm e_depart;
	struct ruter_string destination;
	struct ruter_string line_name;
	struct ruter_string platform;
	struct ruter_departure *next;
};

/**
 * ruter_departure_free() - Frees a departure and all its fields.
 *
 * @event:	Pointer to departure.
 *
 * Recursively frees a departure and everything the departure structure points
 * to. Fields are not zeroed and may be readable after calling this function.
 * Take care not to do that.
 */
void
ruter_departure_free(struct ruter_departure *event);

/**
 * ruter_departure_parse() - Builds a departure structure from JSON data.
 *
 * @data:	Pointer to JSON data.
 *
 * JSON data is extracted and placed inside more intuitive data structures.
 * Calling this function will often produce a graph, so remember to
 * check the next pointer.
 *
 * Return: Pointer to list of departures on success or NULL on failure.
 */
struct ruter_departure
*ruter_departure_parse(json_value *data);

#endif
