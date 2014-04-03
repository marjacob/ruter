#ifndef __H_RUTER_DEPARTURE__
#define __H_RUTER_DEPARTURE__

#include <time.h>
#include "constants.h"
#include "json.h"
#include "wstr.h"

/**
 * struct ruter_departure - Describes a departure.
 *
 * @congested:	Indicates whether a departure is affected by congestion.
 * @vehicle:	Describes the kind of stop this is.
 * @next	Next departure in the linked list.
 * @a_arrive:	Aimed arrival time.
 * @a_depart:	Aimed departure time.
 * @e_arrive:	Expected arrival time.
 * @e_depart:	Expected departure time.
 * @dest:	Name of the destination.
 * @line_name:	Name of the line.
 * @platform	Departure platform.
 *
 * This structure describes the various properties of a departure.
 */
struct ruter_departure {
	int congested;
	vehicle_t vehicle;
	struct ruter_departure *next;
	struct tm a_arrive;
	struct tm a_depart;
	struct tm e_arrive;
	struct tm e_depart;
	wstr_t *dest;
	wstr_t *line_name;
	wstr_t *platform;
};

typedef struct ruter_departure departure_t;

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
ruter_departure_free(departure_t *event);

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
departure_t
*ruter_departure_parse(json_value *data);

#endif

