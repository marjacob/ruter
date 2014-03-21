#include <inttypes.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>
#include "args/args.h"
#include "args/csrc.h"
#include "args/token.h"
#include "json.h"
#include "ruter/ruter.h"
#include "wstr.h"

static int find(ruter_t *session, char *place);

static int show(ruter_t *session, char *place);

static int travel(ruter_t *session, char *origin, char *dest);

int main(int argc, char *argv[])
{	
	if (!setlocale(LC_ALL, "")) {
		fprintf(stderr, 
			"%s: setlocale() failed\n", 
			argv[0]);
		return EXIT_FAILURE;
	}
	
	if (fwide(stdout, 1) <= 0) {
		fprintf(stderr,
			"%s: fwide() failed\n",
			argv[0]);
		return EXIT_FAILURE;
	}

	ruter_t session;
	
	if (!ruter_open(&session, 0)) {
		wprintf(L"%s: ruter_init() failed\n", argv[0]);
		return EXIT_FAILURE;
	}
	
	struct args args;
	
	if (!args_parse(&args, argc, argv)) {
		wprintf(L"%s: args_parse() failed\n");
		ruter_close(&session);
		return EXIT_FAILURE;
	}

	if (args.from && args.to) {
		travel(&session, args.from, args.to);
	} else if (args.find) {
		find(&session, args.find);
	} else if (args.show) {
		show(&session, args.show);
	}
	
	args_free(&args);
	ruter_close(&session);
	
	return EXIT_SUCCESS;
}
static int64_t request_stop(ruter_t *session, char *place)
{
	struct ruter_stop *stops = ruter_find(session, place);
	struct ruter_stop *stop = stops;
	wchar_t buf[8];
	
	while (stop) {
		if (PT_STOP == stop->type) {
			wprintf(
				L"%ls (%ls) [y/n]: ", 
				wstr_ptr(stop->name), 
				wstr_ptr(stop->district));
				
			size_t len = sizeof(buf) / sizeof(wchar_t);
			
			if (!fgetws(buf, len, stdin)) {
				continue;
			} else if (L'y' == buf[0]) {
				break;
			}
		}
		stop = stop->next;
	}
	
	int64_t stop_id = stop ? stop->id : 0;
	ruter_stop_free(stops);
	
	return stop_id;
}

static void print_stops(struct ruter_stop *stops, int level)
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

static void print_events(struct ruter_departure *dep)
{
	struct ruter_departure *cur = dep;
	size_t max_dest = 0;
	size_t max_line = 0;
	
	while (cur) {
		max_dest = wstr_len(cur->dest) > max_dest
			? wstr_len(cur->dest)
			: max_dest;
		max_line = wstr_len(cur->line_name) > max_line
			? wstr_len(cur->line_name)
			: max_line;
		cur = cur->next;
	}
	
	cur = dep;
	wchar_t meta_format[] = L"%%%zuls | %%5ls | %%2ls | %%%zuls | ";
	wchar_t format[512];
	swprintf(format, 512, meta_format, max_dest, max_line);
	
	while (cur) {
		wprintf(
			format,
			wstr_ptr(cur->dest),
			VM_BUS == cur->vehicle
				? L"Bus" :
			VM_FERRY == cur->vehicle
				? L"Ferry" :
			VM_RAIL == cur->vehicle
				? L"Rail" :
			VM_TRAM == cur->vehicle
				? L"Tram" :
			VM_METRO == cur->vehicle
				? L"Metro" : L"N/A",			
			!wstr_ptr(cur->platform)
				? L"" : wstr_ptr(cur->platform), 
			wstr_ptr(cur->line_name));
		wprintf(
			L"%02d:%02d (%02d:%02d) | %02d:%02d (%02d:%02d)\n", 
			cur->a_arrive.tm_hour, 
			cur->a_arrive.tm_min,
			cur->e_arrive.tm_hour, 
			cur->e_arrive.tm_min,
			cur->a_depart.tm_hour, 
			cur->a_depart.tm_min,
			cur->e_depart.tm_hour, 
			cur->e_depart.tm_min);
		cur = cur->next;
	}
}

static int find(ruter_t *session, char *place)
{
	struct ruter_stop *stops = ruter_find(session, place);
	
	if (!stops) {
		wprintf(L"no stops found\n");
		return 0;
	}
	
	print_stops(stops, 0);
	ruter_stop_free(stops);
	
	return 0;
}

static int show(ruter_t *session, char *place)
{
	int64_t stop_id = request_stop(session, place);
	
	if (!stop_id) {
		return 0;
	}
		
	struct ruter_departure *deps = ruter_departures(session, stop_id);
	
	if (!deps) {
		wprintf(L"no realtime events found\n");
		return 0;
	}
	
	print_events(deps);
	ruter_departure_free(deps);
	
	return 0;
}

static int travel(ruter_t *session, char *origin, char *dest)
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
	
	struct ruter_travel *proposals = ruter_travel(
		session, timeinfo, 1, origin_id, dest_id);
	
	ruter_travel_free(proposals);
	
	return 0;
}

