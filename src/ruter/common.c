#include "ruter/common.h"

wchar_t
*wchar(const char *s, size_t n)
{
	if (!s) {
		return NULL;
	}
	
	n = n > 0 ? n : strlen(s);
	mbstate_t ps = { 0 };
	const char *src = s;
	size_t len = 0;
	
	wchar_t *str = malloc(sizeof(*str) * (n + 1));
	
	if (str) {
		if (0 > (len = mbsrtowcs(str, &src, n, &ps))) {
			free(str);
			str = NULL;
		} else {
			str[len] = L'\0';
		}
	}

	return str;
}

bool parse_area(Place *area, const json_value *json)
{
	if (!(area && json && json_object == json->type)) {
		return false;
	}
	
	char *name = NULL;
	json_value *value = NULL;
	
	for (size_t i = 0, j = json->u.object.length; i < j; i++) {
		name = json->u.object.values[i].name;
		value = json->u.object.values[i].value;
		
		if (!strcmp("Stops", name)) {
			value = value;
			return false; /* TODO */
		}
	}
	
	return true;
}

bool parse_poi(Place *poi, const json_value *json)
{
	if (!(poi && json && json_object == json->type)) {
		return false;
	}
	
	char *name = NULL;
	json_value *value = NULL;
	
	for (size_t i = 0, j = json->u.object.length; i < j; i++) {
		name = json->u.object.values[i].name;
		value = json->u.object.values[i].value;
		
		if (!strcmp("X", name)) {
			poi->u.Stop.Location.X = value->u.integer;
		} else if (!strcmp("Y", name)) {
			poi->u.Stop.Location.Y = value->u.integer;
		}
	}
	
	return true;
}

bool parse_stop(Place *stop, const json_value *json)
{
	if (!(stop && json && json_object == json->type)) {
		return false;
	}
	
	char *name = NULL;
	json_value *value = NULL;
	
	for (size_t i = 0, j = json->u.object.length; i < j; i++) {
		name = json->u.object.values[i].name;
		value = json->u.object.values[i].value;
		
		if (!strcmp("ShortName", name)) {
			stop->u.Stop.ShortName = jsontowcs(value);
		} else if (!strcmp("Zone", name)) {
			stop->u.Stop.Zone = jsontowcs(value);
		} else if (!strcmp("IsHub", name)) {
			stop->u.Stop.IsHub = value->u.boolean;
		} else if (!strcmp("AlightingAllowed", name)) {
			stop->u.Stop.AlightingAllowed = value->u.boolean;
		} else if (!strcmp("BoardingAllowed", name)) {
			stop->u.Stop.BoardingAllowed = value->u.boolean;
		} else if (!strcmp("RealTimeStop", name)) {
			stop->u.Stop.RealTimeStop = value->u.boolean;
		} else if (!strcmp("X", name)) {
			stop->u.Stop.Location.X = value->u.integer;
		} else if (!strcmp("Y", name)) {
			stop->u.Stop.Location.Y = value->u.integer;
		}
	}
	
	return true;
}

bool parse_street(Place *street, const json_value *json)
{
	if (!(street && json && json_object == json->type)) {
		return false;
	}
	
	char *name = NULL;
	json_value *value = NULL;
	
	for (size_t i = 0, j = json->u.object.length; i < j; i++) {
		name = json->u.object.values[i].name;
		value = json->u.object.values[i].value;
		
		if (!strcmp("Houses", name)) {
			value = value;
			return false; /* TODO */
		}
	}
	
	return true;
}

inline static PlaceType get_place_type(const json_value *json)
{
	char *type = NULL;
	
	if (json && json_string == json->type) {
		type = json->u.string.ptr;
		
		if (!strcmp("Stop", type)) {
			return RPT_STOP;
		} else if (!strcmp("Area", type)) {
			return RPT_AREA;
		} else if (!strcmp("POI", type)) {
			return RPT_POI;
		} else if (!strcmp("Street", type)) {
			return RPT_STREET;
		}
	}
	
	return RPT_NONE;
}

bool parse_place(Place *place, const json_value *json)
{
	if (!(place && json && json_object == json->type)) {
		return false;
	}
	
	char *name = NULL;
	json_value *value = NULL;
	
	for (size_t i = 0, j = json->u.object.length; i < j; i++) {
		name = json->u.object.values[i].name;
		value = json->u.object.values[i].value;
		
		if (!strcmp("ID", name) && !(place->ID = value->u.integer)) {
			return false;
		} else if (!strcmp("Name", name)) {
			place->Name = jsontowcs(value);
		} else if (!strcmp("District", name)) {
			place->District = jsontowcs(value);
		} else if (!strcmp("PlaceType", name)) {
			switch ((place->Type = get_place_type(value))) {
			case RPT_AREA:
				return parse_area(place, json);
			case RPT_POI:
				return parse_poi(place, json);
			case RPT_STOP:
				return parse_stop(place, json);
			case RPT_STREET:
				return parse_street(place, json);
			default:
				return false;
			}
		}
	}
	
	return true;
}

bool parse_places(Place *place, size_t n, const json_value *json)
{
	if (!(place && json && json_array == json->type)) {
		return false;
	}
	
	for (size_t i = 0; i < n; i++) {
		place->index = n - i - 1;
		parse_place(place++, json->u.array.values[i]);
	}
	
	return true;
}
