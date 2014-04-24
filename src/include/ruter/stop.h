#ifndef __H_RUTER_STOP__
#define __H_RUTER_STOP__

#include <inttypes.h>
#include "constants.h"
#include "json.h"
#include "ruter/line.h"
#include "wstr.h"

/**
 * struct ruter_stop - Describes a stop.
 *
 * @id:		Unique stop ID.
 * @type:	Describes the kind of stop this is.
 * @realtime	Non-zero if the stop supports realtime traffic information.
 * @district	Name of the district where the stop is located.
 * @name	Name of the stop.
 * @zone	Zone where the stop is located.
 * @lines	All lines going through the stop.
 * @next	Next stop in the linked list.
 * @stops	All stops belonging to an area.
 *
 * This structure describes the various properties of a stop. The type of the
 * stop influences which of the properties are available. The 'stops' field
 * for example is usually only non-NULL if type is 'POI'. These details has 
 * not been documented in the official API though.
 */
struct ruter_stop {
	int64_t id;
	place_t type;
	int realtime;
	wstr_t *district;
	wstr_t *name;
	wstr_t *zone;
	struct ruter_line *lines;
	struct ruter_stop *next;
	struct ruter_stop *stops;
};

typedef struct ruter_stop stop_t;

/**
 * ruter_stop_copy() - Copies a stop and all its fields.
 *
 * @stop:	Pointer to stop.
 *
 * Recursively copies a stop and everything the stop structure points to.
 */
stop_t
*ruter_stop_copy(const stop_t *stop);

/**
 * ruter_stop_free() - Frees a stop and all its fields.
 *
 * @stop:	Pointer to stop.
 *
 * Recursively frees a stop and everything the stop structure points to.
 * Fields are not zeroed and may be readable after calling this function.
 * Take care not to do that.
 */
void
ruter_stop_free(stop_t *stop);

/**
 * ruter_stop_mode() - Returns the mode of transportation for a stop.
 *
 * @stop:	Pointer to stop.
 *
 * Attempts to guess the mode of transportation by looking for known clues
 * in the name of the stop, as Ruter encodes this information by adding it
 * as tags in the stop name.
 *
 * Return: A value of type vehicle_t indicating the mode of transportation.
 */
vehicle_t
ruter_stop_mode(const stop_t *stop);

/**
 * ruter_stop_parse() - Builds a stop structure from JSON data.
 *
 * @data:	Pointer to JSON data.
 *
 * JSON data is extracted and placed inside more intuitive data structures.
 * Calling this function will often produce a graph, so remember to
 * check the next pointer and stops pointer.
 *
 * Return: Pointer to graph of stops on success or NULL on failure.
 */
stop_t
*ruter_stop_parse(const json_value *data);

/**
 * ruter_stop_match() - Checks the mode of transportation for a stop.
 *
 * @stop:	Pointer to stop.
 *
 * Checks whether a stop supports a given mode of transportation.
 *
 * Return: A non-zero value if the stop supports the mode of transportation.
 */
inline static uint8_t
ruter_stop_match(const stop_t *stop, vehicle_t mode)
{
	return stop
		&& (PT_STOP == stop->type)
		&& (VM_NONE == mode || ruter_stop_mode(stop) == mode);
}

#endif

