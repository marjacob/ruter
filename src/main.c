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

static int find_rec(json_value *data, int indent)
{
	switch (data->type) {
		case json_object:
			for (int i = 0; i < data->u.object.length; i++) {
				// data->u.object.values[i].name
				find_rec(data->u.object.values[i].value, indent + 2);
			}
			break;
		case json_array:
			for (int i = 0; i < data->u.array.length; i++) {
				find_rec(data->u.array.values[i], indent + 2);
			}
			break;
		case json_integer:
			// data->u.integer
			break;
		case json_double:
			// data->u.dbl
			break;
		case json_string:
			for (int i = 0; i < indent; i++) {
				printf(" ");
			}
			printf("%s\n", data->u.string.ptr);
			//data->u.string.ptr
			//data->u.string.length
			break;
		case json_boolean:
			// data->u.boolean
			break;
		default:
			// covers json_null, json_none
			break;
	}
	return 0;
}

static int find(struct ruter_session *session, char *place)
{
	ruter_find(session, place);
	
	json_value *districts = json_parse(session->buf, session->bufsize);
	
	find_rec(districts, 0);
	
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
