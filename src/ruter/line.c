#include <stdlib.h>
#include <string.h>
#include "json.h"
#include "ruter/line.h"
#include "ruter/util.h"

static struct ruter_line
*ruter_line_array_parse(json_value *data);

static struct ruter_line line_zero = { 0 };

struct ruter_line
*ruter_line_init(void)
{
	struct ruter_line *line = malloc(sizeof(*line));
	*line = line_zero;
	return line;
}

void
ruter_line_free(struct ruter_line *line)
{
	if (NULL != line) {
		ruter_line_free(line->next);
		ruter_safe_free(line->name);
		free(line);
	}
}

struct ruter_line
*ruter_line_parse(json_value *data)
{
	if (NULL == data) {
		return NULL;
	} else if (json_array == data->type) {
		return ruter_line_array_parse(data);
	} else if (json_object != data->type) {
		return NULL;
	}

	char *name = NULL;
	json_value *value = NULL;
	struct ruter_line *line = ruter_line_init();

	for (int i = 0, j = data->u.object.length; i < j; i++) {
		name = data->u.object.values[i].name;
		value = data->u.object.values[i].value;

		if (0 == strcmp("LineID", name)) {
			if (0 == (line->id = value->u.integer)) {
				ruter_line_free(line);
				return NULL;
			}
		} else if (0 == strcmp("LineName", name)) {
			line->name = ruter_strndup(
				value->u.string.ptr,
				value->u.string.length);
		} else if (0 == strcmp("Transportation", name)) {
			line->type = (enum transport_type)value->u.integer;
		}
	}

	return line;
}

static struct ruter_line
*ruter_line_array_parse(json_value *data)
{
	if (NULL == data || json_array != data->type) {
		return NULL;
	}

	struct ruter_line *line = NULL;
	struct ruter_line *lines = NULL;
	struct ruter_line *last_line = NULL;

	for (int i = 0, j = data->u.array.length; i < j; i++) {
		line = ruter_line_parse(data->u.array.values[i]);

		if (NULL == line) {
			continue;
		} else if (NULL == lines) {
			lines = line;
		} else {
			last_line->next = line;
		}

		last_line = line;
	}

	return lines;
}
