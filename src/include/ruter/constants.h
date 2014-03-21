#ifndef __H_RUTER_CONSTANTS__
#define __H_RUTER_CONSTANTS__

/* Initial size of the data receive buffer. 
 * The buffer size is increased automatically when necessary. */
#ifndef RUTER_BUFFER_SIZE
#	define RUTER_BUFFER_SIZE 8192
#endif

#ifndef RUTER_API_URI
#	define RUTER_API_URI "https://reis.trafikanten.no/ReisRest"
#endif

#ifndef RUTER_API_LENGTH
#	define RUTER_API_LENGTH (sizeof(RUTER_API_URI))
#endif

#ifndef RUTER_USER_AGENT
#	define RUTER_USER_AGENT "PosixRuterCLI/0.4-alpha"
#endif

/**
 * enum place_type - Various kinds of stops.
 *
 * @PT_STOP:	A regular stop for public transportation.
 * @PT_AREA:	An area that may contain stops.
 * @PT_POI:	A point of interest that may have stops nearby.
 * @PT_STREET:	A street that may have stops nearby.
 *
 * These are the various kinds of stops available through the API.
 */
enum place_type {
	PT_STOP = 0,
	PT_AREA = 1,
	PT_POI = 2,
	PT_STREET = 3
};

/**
 * enum transport_type - Various methods of transportation.
 *
 * @TT_WALKING:		Usually indicates a short walk between stops.
 * @TT_AIRPORT_BUS:	Transport to an airport available by bus.
 * @TT_BUS:		Transport available by bus.
 * @TT_DUMMY:		Probably used internally by the servers.
 * @TT_AIRPORT_TRAIN:	Transport to an airport available by train.
 * @TT_BOAT:		Transport available by boat.
 * @TT_TRAIN:		Transport available by train.
 * @TT_TRAM:		Transport available by tram.
 * @TT_METRO:		Transport available by metro.
 *
 * These are the various methods of transportation available trough the 
 * travel planner API.
 */
enum transport_type {
	TT_WALKING = 0,
	TT_AIRPORT_BUS = 1,
	TT_BUS = 2,
	TT_DUMMY = 3,
	TT_AIRPORT_TRAIN = 4,
	TT_BOAT = 5,
	TT_TRAIN = 6,
	TT_TRAM = 7,
	TT_METRO = 8
};
 
/**
 * enum vehicle_mode - Various methods of transportation.
 *
 * @VM_BUS:	Realtime information available for bus.
 * @VM_FERRY:	Realtime information available for ferry.
 * @VM_RAIL:	Realtime information available for rail.
 * @VM_TRAM:	Realtime information available for tram.
 * @VM_METRO:	Realtime information available for metro.
 *
 * These are the various methods of transportation available through the 
 * realtime systems API.
 */
enum vehicle_mode {
	VM_BUS = 0,
	VM_FERRY = 1,
	VM_RAIL = 2,
	VM_TRAM = 3,
	VM_METRO = 4
};

/* About the data model:
 * http://labs.ruter.no/how-to-use-the-api/about-the-data-model.aspx */

#endif

