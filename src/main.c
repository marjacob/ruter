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
	
	printf("\n");
	
	print_stops(stops->stops, level + 1);
	print_stops(stops->next, level);
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
	if (ruter_is_realtime(session, place)) {
		printf("YES, %s IS REALTIME\n", place);
	}
	
	return 0;
}

static int travel(struct ruter_session *session, char *origin, char *dest)
{
	printf("Travel: %s ---> %s\n", origin, dest);
	
	return 0;
}
