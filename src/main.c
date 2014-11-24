#include <getopt.h>
#include <inttypes.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>
#include "main.h"
#include "ruter/constants.h"
#include "ruter/json.h"
#include "ruter/ruter.h"
#include "table.h"
#include "util.h"
#include "wstr.h"

static int
find(ruter_t *session, const args_t *args);

static int
show(ruter_t *session, args_t *args);

static int
travel(ruter_t *session, char *origin, char *dest);

static void
write_line(wchar_t *ptr, size_t len, void *user)
{
	wprintf(L"%ls\n", ptr);
}

inline static void
args_init(args_t *args)
{
	if (args) {
		args->from = NULL;
		args->search = NULL;
		args->to = NULL;
		args->limit = 0;
		args->kind = VM_NONE;
	}
}

int
main(int argc, char *argv[])
{	
	if (!setlocale(LC_ALL, "")) {
		fprintf(stderr, "%s: setlocale() failed\n", argv[0]);
		return EXIT_FAILURE;
	}
	
	if (fwide(stdout, 1) <= 0) {
		fprintf(stderr, "%s: fwide() failed\n", argv[0]);
		return EXIT_FAILURE;
	}
	
	/* Interpret command line arguments. */
	static char shortopt[] = "f:k:l:s:t:";
	static struct option longopt[] =
	{
		{"from",   required_argument, NULL, 'f'},
		{"kind",   required_argument, NULL, 'k'},
		{"limit",  required_argument, NULL, 'l'},
		{"search", required_argument, NULL, 's'},
		{"to",     required_argument, NULL, 't'},
		{NULL, 0, NULL, 0}
	};
	
	int c;
	args_t args;
	args_init(&args);
	
	while (-1 != (c = getopt_long(argc, argv, shortopt, longopt, NULL))) {
		switch (c) {
		case 'f': /* --from */
			args.from = optarg;
			break;
		case 'k': /* --kind */
			args.kind = util_vehicle_mode(optarg);
			break;
		case 'l': /* --limit */
			args.limit = (size_t)atoll(optarg);
			break;
		case 's': /* --search*/
			args.search = optarg;
			break;
		case 't': /* --to */
			args.to = optarg;
			break;
		}
	}

	ruter_t session;
	
	if (!ruter_open(&session, 0)) {
		wprintf(L"%s: ruter_init() failed\n", argv[0]);
		return EXIT_FAILURE;
	}
	
	if (args.from && args.to) {
		travel(&session, args.from, args.to);
	} else if (args.search) {
		find(&session, &args);
	} else if (args.from || args.to) {
		show(&session, &args);
	}

	ruter_close(&session);
	
	return EXIT_SUCCESS;
}

static int64_t
request_stop(ruter_t *session, char *place)
{
	stop_t *head = ruter_find(session, place);
	int64_t id = 0;
	wchar_t buf[8];
	
	for (stop_t *stop = head; stop->next; stop = stop->next) {
		if (PT_STOP == stop->type) {
			wprintf(
				L"%ls (%ls) [y/n]: ", 
				wstr_ptr(stop->name), 
				wstr_ptr(stop->district));
				
			size_t len = sizeof(buf) / sizeof(wchar_t);
			
			if (!fgetws(buf, len, stdin)) {
				continue;
			} else if (L'y' == buf[0]) {
				id = stop->id;
				break;
			}
		}
	}
	
	ruter_stop_free(head);
	return id;
}

static void
print_stops(stop_t *stops, int level)
{
	if (!stops) {
		return;
	}
	
	for (int i = 0; i < level; i++) {
		wprintf(L"\t");
	}
	
	wprintf(L"%" PRId64 L": %ls", stops->id, wstr_ptr(stops->name));
	
	if (!wstr_empty(stops->zone) && !wstr_empty(stops->district)) {
		wprintf(
			L" (%ls, %ls)", 
			wstr_ptr(stops->zone),
			wstr_ptr(stops->district));
	} else if (!wstr_empty(stops->zone)) {
		wprintf(L" (%ls)", wstr_ptr(stops->zone));
	} else if (!wstr_empty(stops->district)) {
		wprintf(L" (%ls)", wstr_ptr(stops->district));
	}
	
	wprintf(L"\n");

	print_stops(stops->stops, level + 1);
	print_stops(stops->next, level);
}

static void
print_events(departure_t *dep, const stop_t *stop, args_t *args)
{
	table_t *table = NULL;
	size_t rows = 0;
	
	/* Compute number of rows. */
	for (departure_t *cur = dep; cur; cur = cur->next) {
		rows++;
	}
	
	/* Initialise table and set its headers. */
	table = table_create(5, rows, write_line);
	table_set_title(table, wstr_ptr(stop->name));
	table_set_limit(table, args->limit);
	table_set_user(table, args);
	table_set_header(
		table, 
		L"Destination", 
		L"Line", 
		L"Track", 
		args->from ? L"Departure" : L"Arrival",
		L"Type");
	
	size_t row = 0;
	wchar_t *time = malloc(sizeof(*time) * 14 * rows);
	wchar_t *type = malloc(sizeof(*type) * 6 * rows);
	wchar_t *ptime = time;
	wchar_t *ptype = type;
	struct tm *schedule = NULL;
	struct tm *expected = NULL;
	
	/* Format data and add rows. */
	for (departure_t *cur = dep; cur; cur = cur->next) {
		schedule = args->from ? &cur->a_depart : &cur->a_arrive;
		expected = args->from ? &cur->e_depart : &cur->e_arrive;
		
		/* Format departure or arrival time. */
		swprintf(ptime, 14, L"%02d:%02d (%02d:%02d)", 
			schedule->tm_hour, 
			schedule->tm_min,
			expected->tm_hour, 
			expected->tm_min);
		
		/* Obtain raw pointers and insert row. */
		table_set_row(
			table, 
			row++,
			wstr_ptr(cur->dest),
			wstr_ptr(cur->line_name),
			wstr_ptr(cur->platform),
			ptime,
			util_vehicle_text(cur->vehicle, 1));
		
		ptime += 14;
		ptype += 6;
	}
	
	table_print(table);
	table_free(table);
	free(time);
	free(type);
}

static int
find(ruter_t *session, const args_t *args)
{
	stop_t *stops = ruter_find(session, args->search);
	
	if (!stops) {
		wprintf(L"no stops found\n");
		return 0;
	}
	
	print_stops(stops, 0);
	ruter_stop_free(stops);
	
	return 0;
}

static int
show(ruter_t *session, args_t *args)
{
	char *place = args->from ? args->from : args->to;
	stop_t *stop = ruter_guess(session, place, args->kind);
	
	if (!stop) {
		return 0;
	}
	
	departure_t *deps = ruter_departures(session, stop->id);
	
	if (!deps) {
		ruter_stop_free(stop);
		wprintf(L"no realtime events found\n");
		return 0;
	}
	
	print_events(deps, stop, args);
	ruter_stop_free(stop);
	ruter_departure_free(deps);
	
	return 0;
}

static int
travel(ruter_t *session, char *origin, char *dest)
{
	int64_t origin_id = request_stop(session, origin);
	int64_t dest_id = request_stop(session, dest);
	
	if (!origin_id || !dest_id) {
		return 0;
	}
	
	time_t rawtime;
  	struct tm *timeinfo;

  	time(&rawtime);
  	timeinfo = localtime(&rawtime);
	
	proposal_t *proposals = ruter_travel(
		session, timeinfo, 1, origin_id, dest_id);
	
	for (proposal_t *p = proposals; p; p = p->next) {
		
	}

	ruter_travel_free(proposals);
	
	return 0;
}

