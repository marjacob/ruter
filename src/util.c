#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "ruter/constants.h"
#include "util.h"

char
*util_strdup(const char *s)
{
	size_t len = strlen(s);
	char *dup = malloc(len + 1);
	
	if (!dup) {
		return NULL;
	} else {
		dup[len] = '\0';
	}
	
	return memcpy(dup, s, len);

}

char
*util_strndup(char const *s, size_t n)
{
	size_t len = util_strnlen(s, n);
	char *dup = malloc(len + 1);
	
	if (!dup) {
		return NULL;
	} else {
		dup[len] = '\0';
	}
	
	return memcpy(dup, s, len);
}

size_t
util_strnlen(const char *s, size_t maxlen)
{
	register const char *p;
	for (p = s; *p && maxlen--; ++p);
	return(p - s);
}

vehicle_t
util_vehicle_mode(const char *s)
{
	if (!strncmp("bus", s, 3)) {
		return VM_BUS;
	} else if (!strncmp("ferry", s, 5)) {
		return VM_FERRY;
	} else if (!strncmp("rail", s, 4) || !strncmp("train", s, 5)) {
		return VM_RAIL;
	} else if (!strncmp("tram", s, 4)) {
		return VM_TRAM;
	} else if (!strncmp("metro", s, 5)) {
		return VM_METRO;
	}

	return VM_NONE;
}

char
*util_vehicle_text(vehicle_t vehicle, int wchar)
{
	switch (vehicle) {
	case VM_BUS:
		return wchar ? (char *)L"bus" : "bus";
	case VM_FERRY:
		return wchar ? (char *)L"ferry" : "ferry";
	case VM_RAIL:
		return wchar ? (char *)L"train" : "train";
	case VM_TRAM:
		return wchar ? (char *)L"tram" : "tram";
	case VM_METRO:
		return wchar ? (char *)L"metro" : "metro";
	default:
		break;
	}

	return wchar ? (char *)L"" : "";
}

