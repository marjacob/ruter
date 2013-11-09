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

static int fill_stop(json_value *data, struct ruter_stop *stop)
{
	char *name = NULL;
	json_value *value = NULL;
	enum place_type type = PT_STOP;
	
	for (int i = 0, j = data->u.object.length; i < j; i++) {
		name = data->u.object.values[i].name;
		value = data->u.object.values[i].value;
		
		if (0 == strcmp("ID", name)) {
			stop->id = value->u.integer;
		} else if (0 == strcmp("District", name)) {
			stop->district = value->u.string.ptr;
		} else if (0 == strcmp("Name", name)) {
			stop->name = value->u.string.ptr;
		} else if (0 == strcmp("Zone", name)) {
			stop->zone = value->u.string.ptr;
		} else if (0 == strcmp("Type", name)) {
			type = (enum place_type)value->u.integer;
		}
	}
	
	return (PT_STOP == type);
}

static int find(struct ruter_session *session, char *place)
{
	ruter_find(session, place);
	
	json_value *districts = json_parse(session->buf, session->bufsize);
	struct ruter_stop *stops = NULL, *stop = NULL, *last = NULL;
	
	if (NULL == districts || json_array != districts->type) {
		printf("no results\n");
		return 0;
	}
	
	for (int i = 0, j = districts->u.array.length; i < j; i++) {
		stop = malloc(sizeof(*stop));
		if (fill_stop(districts->u.array.values[i], stop)) {
			if (NULL == stops) {
				stops = stop;
			}
			
			if (NULL != last) {
				last->next = stop;
			}
			
			last = stop;
		} else {
			free(stop);
		}
	}
	
	for (stop = stops; NULL != stop;) {
		printf(
			"%" PRIi64 ": %s/%s (Zone: %s)\n",
			stop->id,
			stop->district,
			stop->name,
			stop->zone
		);
		
		last = stop;
		stop = stop->next;
		free(last);
	}
	
	json_value_free(districts);
	
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
