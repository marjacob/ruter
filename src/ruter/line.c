#include <stdlib.h>
#include <string.h>
#include "json.h"
#include "ruter/line.h"
#include "ruter/util.h"

static line_t line_zero = { 0 };

inline static line_t
*line_init(void)
{
	line_t *line = malloc(sizeof(*line));
	*line = line_zero;
	return line;
}

static line_t
*line_array_parse(json_value *data)
{
	if (!is_json_array(data)) {
		return NULL;
	}

	line_t *curr = NULL;
	line_t *head = NULL;
	line_t *tail = NULL;

	for (int i = 0, j = data->u.array.length; i < j; i++) {
		curr = ruter_line_parse(data->u.array.values[i]);

		if (!curr) {
			continue;
		} else if (!head) {
			head= curr;
		} else {
			tail->next = curr;
		}

		tail = curr;
	}

	return head;
}

line_t
*ruter_line_copy(const line_t *line)
{
	line_t *copy = NULL;
	
	if (line) {
		copy = malloc(sizeof(*copy));
		memcpy(copy, line, sizeof(*copy));
		copy->next = ruter_line_copy(copy->next);
	}
	
	return copy;
}

void
ruter_line_free(line_t *line)
{
	if (line) {
		ruter_line_free(line->next);
		wstr_free(line->name);
		free(line);
	}
}

line_t
*ruter_line_parse(json_value *data)
{
	if (is_json_array(data)) {
		return line_array_parse(data);
	} else if (!is_json_object(data)) {
		return NULL;
	}

	char *name = NULL;
	json_value *value = NULL;
	line_t *line = line_init();

	for (int i = 0, j = data->u.object.length; i < j; i++) {
		name = data->u.object.values[i].name;
		value = data->u.object.values[i].value;
		line->id = value->u.integer;

		if (!strcmp("LineID", name) && !line->id) {
			ruter_line_free(line);
			return NULL;
		} else if (!strcmp("LineName", name)) {
			line->name = wstr_from_json(value);
		} else if (!strcmp("Transportation", name)) {
			line->type = value->u.integer;
		}
	}

	return line;
}

