#include <stdlib.h>
#include <string.h>
#include "json.h"
#include "ruter/line.h"
#include "ruter/util.h"

static struct ruter_line line_zero = { 0 };

inline static struct ruter_line
*ruter_line_init(void)
{
	struct ruter_line *line = malloc(sizeof(*line));
	*line = line_zero;
	return line;
}

static struct ruter_line
*ruter_line_array_parse(json_value *data)
{
	if (!is_json_array(data)) {
		return NULL;
	}

	struct ruter_line
		*line = NULL,
		*lines = NULL,
		*last_line = NULL;

	for (int i = 0, j = data->u.array.length; i < j; i++) {
		line = ruter_line_parse(data->u.array.values[i]);

		if (!line) {
			continue;
		} else if (!lines) {
			lines = line;
		} else {
			last_line->next = line;
		}

		last_line = line;
	}

	return lines;
}

void
ruter_line_free(struct ruter_line *line)
{
	if (line) {
		ruter_line_free(line->next);
		wstr_free(line->name);
		free(line);
	}
}

struct ruter_line
*ruter_line_parse(json_value *data)
{
	if (is_json_array(data)) {
		return ruter_line_array_parse(data);
	} else if (!is_json_object(data)) {
		return NULL;
	}

	char *name = NULL;
	json_value *value = NULL;
	struct ruter_line *line = ruter_line_init();

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

