#ifndef __H_RUTER_CONSTANTS__
#define __H_RUTER_CONSTANTS__

#ifndef RUTER_BUFFER_SIZE
#	define RUTER_BUFFER_SIZE 8192
#endif

#ifndef RUTER_API_URI
#	define RUTER_API_URI "http://api.ruter.no/ReisRest"
#endif

#ifndef RUTER_API_LENGTH
#	define RUTER_API_LENGTH (sizeof(RUTER_API_URI))
#endif

#ifndef RUTER_USER_AGENT
#	define RUTER_USER_AGENT "PosixRuterCLI/0.3-pre-alpha"
#endif

#ifndef RUTER_URI_SPACE
#	define RUTER_URI_SPACE "%20"
#endif

/* See http://labs.ruter.no/how-to-use-the-api/about-the-data-model.aspx */

enum place_type {
	PT_STOP = 0,
	PT_AREA = 1,
	PT_POI = 2,
	PT_STREET = 3
};

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
 
enum vehicle_mode {
	VM_BUS = 0,
	VM_FERRY = 1,
	VM_RAIL = 2,
	VM_TRAM = 3,
	VM_METRO = 4
};

#endif
