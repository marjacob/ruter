#ifndef __H_RUTER_API__
#define __H_RUTER_API__

#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>
#include "ruter/session.h"

/**
 * PlaceType - Various kinds of stops.
 *
 * @PT_NONE:	Unknown or no stop type.
 * @PT_STOP:	A regular stop for public transportation.
 * @PT_AREA:	An area that may contain stops.
 * @PT_POI:	A point of interest that may have stops nearby.
 * @PT_STREET:	A street that may have stops nearby.
 *
 * These are the various kinds of stops available through the API.
 */
typedef enum {
	RPT_NONE = -1,
	RPT_STOP = 0,
	RPT_AREA = 1,
	RPT_POI = 2,
	RPT_STREET = 3
} PlaceType;

/**
 * UTM32 - Describes a location.
 *
 * @X:		X coordinate.
 * @Y:		Y coordinate.
 *
 * This structure represents a UTM32 formatted geographical location with two 
 * unsigned 32 bits integers.
 */
typedef struct {
	uint32_t X;
	uint32_t Y;
} UTM32;

typedef struct {
	uint32_t ID;
	wchar_t *Name;
	wchar_t *District;
	PlaceType Type;
	union {
		struct {
			/* Stops */
		} Area;
		
		struct {
			UTM32 Location;
		} POI;
		
		struct {
			wchar_t *ShortName;
			wchar_t *Zone;
			bool IsHub;
			bool AlightingAllowed;
			bool BoardingAllowed;
			/* Deviations */
			/* Lines */
			/* StopPoints */
			bool RealTimeStop;
			UTM32 Location;
		} Stop;
		
		struct {
			/* Houses */
		} Street;
	} u;
	size_t index;
} Place;

typedef struct {
	wchar_t *DestinationName;
	bool InCongestion;
	struct tm AimedArrivalTime;
	struct tm ExpectedArrivalTime;
	struct tm AimedDepartureTime;
	struct tm ExpectedDepartureTime;
} Departure;

inline static void FreePlace(Place *place)
{
	if (!place) {
		return;
	}
	
	for (size_t i = 0, j = place->index + 1; i < j; i++) {
		free(place[i].Name);
		free(place[i].District);
		
		if (RPT_STOP == place[i].Type) {
			free(place[i].u.Stop.ShortName);
			free(place[i].u.Stop.Zone);
		}
	}
	
	free(place);
}

/**
 * GetStop() - Returns all data about a Stop.
 *
 * @session	Pointer to session structure.
 * @stop:	Stop ID.
 *
 * Uses GetPlaces() internally to obtain all information about a Stop. The
 * official API provides a GetStop() method, which returns incomplete and
 * incompatible results.
 *
 * Return: Place structure describing a Stop, or NULL on failure.
 */
Place
*GetStop(ruter_t *session, uint32_t id);

/**
 * GetPlaces() - Returns a list of Places.
 *
 * @session	Pointer to session structure.
 * @name:	Search string.
 * @location	Optional UTM32 location which can be NULL.
 *
 * Returns a list of Places that have names similar to the search string.
 * If a location is provided, search results are sorted according to 
 * geographical proximity.
 *
 * Return: List of Place structures, or NULL on failure.
 */
Place
*GetPlaces(ruter_t *session, const char *name, const UTM32 *location);

/**
 * GetPlaces() - Returns a List of Departures.
 *
 * @session	Pointer to session structure.
 * @name:	Stop ID.
 * @time	Optional pointer to time structure which can be NULL.
 *
 * Returns a list of Departures associated with the Stop ID and (if specified)
 * time.
 *
 * Return: List of Departure structures, or NULL on failure.
 */
Departure
*GetDepartures(ruter_t *session, uint32_t id, struct tm *time);

/*
http://reisapi.ruter.no/help

ruter_GetStop();
ruter_GetPlaces();
ruter_GetStopsByLineID();
ruter_GetClosestStops();
ruter_GetStopsByArea();
ruter_GetSalePointsByArea();
ruter_GetTrip();
ruter_GetTravels();
ruter_GetDepartures();
ruter_GetStreet();
ruter_GetLines();
ruter_GetLinesByStopID();
ruter_GetDataByLineID();
ruter_GetStopsByLineID();
ruter_GetFavourites();
*/

#endif
