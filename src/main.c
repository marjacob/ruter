#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "json.h"
#include "ruter.h"

static int find(struct ruter_session *session, char *place);

static int show(struct ruter_session *session, char *place);

static int travel(struct ruter_session *session, char *origin, char *dest);

int main(int argc, char *argv[])
{
	extern char *optarg;
	extern int optind;
	int c;
	char *origin = NULL;
	char *destination = NULL;
	char *find_place = NULL;
	char *show_place = NULL;
	
	while ((c = getopt(argc, argv, "d:f:o:s:")) != -1) {
		switch (c) {
		case 'd':
			destination = optarg;
			break;
		case 'f':
			find_place = optarg;
			break;
		case 'o':
			origin = optarg;
			break;
		case 's':
			show_place = optarg;
			break;
		case '?':
			return EXIT_FAILURE;
		}
	}
	
	if (optind < argc) {
		/* these are the arguments after the command-line options */ 
		for (; optind < argc; optind++) {
			printf("argument: \"%s\"\n", argv[optind]);
		}
	}
	
	struct ruter_session session;
	
	if (!ruter_init(&session, 0)) {
		printf("%s: failed to init curl\n", argv[0]);
		return EXIT_FAILURE;
	}
	
	if (NULL != origin && NULL != destination) {
		travel(&session, origin, destination);
	} else if (NULL != find_place) {
		find(&session, find_place);
	} else if (NULL != show_place) {
		show(&session, show_place);
	}
	
	ruter_close(&session);
	
	return EXIT_SUCCESS;
}

static void print_stops(struct ruter_stop *stops, int level)
{
	if (NULL == stops) {
		return;
	}
	
	for (int i = 0; i < level; i++) {
		printf("\t");
	}
	
	switch (stops->type) {
	case PT_STOP:
		printf("Stop: ");
		break;
	case PT_AREA:
		printf("Area: ");
		break;
	case PT_POI:
		printf("POI: ");
		break;
	case PT_STREET:
		printf("Street: ");
		break;
	}
	
	printf("%s", stops->name);
	
	if (NULL != stops->district) {
		printf(" (%s)", stops->district);
	}
	
	if (NULL != stops->zone) {
		printf(" (sone %s)", stops->zone);
	}
	
	if (stops->realtime) {
		printf(" (realtime)");
	}
	
	printf("\n");
	
	print_stops(stops->stops, level + 1);
	print_stops(stops->next, level);
}

static void print_events(struct ruter_departure *events)
{
	if (NULL == events) {
		return;
	}
	
	printf(
		" %5s | %2s | %6s | %s\n",
		VM_BUS == events->vehicle_mode
			? "Bus" :
		VM_FERRY == events->vehicle_mode
			? "Ferry" :
		VM_RAIL == events->vehicle_mode
			? "Rail" :
		VM_TRAM == events->vehicle_mode
			? "Tram" :
		VM_METRO == events->vehicle_mode
			? "Metro" : "N/A",			
		events->platform, 
		events->line_name, 
		events->destination);
	
	print_events(events->next);
}

static int find(struct ruter_session *session, char *place)
{
	struct ruter_stop *stops = ruter_find(session, place);
	
	if (NULL == stops) {
		printf("no stops found\n");
		return 0;
	}
	
	print_stops(stops, 0);
	ruter_stop_free(stops);
	
	return 0;
}

static int show(struct ruter_session *session, char *place)
{
	struct ruter_stop *stops = ruter_find(session, place);
	struct ruter_stop *stop = stops;
	char buf[8];
	
	while (NULL != stop) {
		if (PT_STOP == stop->type) {
			printf("%s (%s) [y/n]: ", stop->name, stop->district);
			if (NULL == fgets(buf, sizeof(buf), stdin)) {
				continue;
			} else if ('y' == buf[0]) {
				break;
			}
		}
		stop = stop->next;
	}
	
	if (NULL == stop) {
		return 0;
	}
	
	struct ruter_departure *deps = ruter_departures(session, stop->id);
	ruter_stop_free(stops);
	
	if (NULL == deps) {
		printf("no realtime events found\n");
		return 0;
	}
	
	print_events(deps);
	ruter_departure_free(deps);
	
	return 0;
}

static int travel(struct ruter_session *session, char *origin, char *dest)
{
	printf("Travel: %s ---> %s\n", origin, dest);
	
	return 0;
}
