#include <inttypes.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "table.h"
#include "util.h"

#define CELL(table, col, row) (table->table[row * table->cols + col])

table_t
*table_create(size_t cols, size_t rows, writelfunc write)
{
	table_t *table = NULL;
	char *table_mem = NULL;
	
	/* Sizes */
	size_t headr_size = sizeof(*table);
	size_t table_size = sizeof(*table->table) * cols * (rows + 1);
	size_t width_size = sizeof(*table->width) * cols;
	size_t total_size = headr_size + table_size + width_size;
	
	/* Offsets */
	size_t table_offset = headr_size;
	size_t width_offset = table_offset + table_size;
	
	if ((table_mem = malloc(total_size))) {
		table = (table_t *)table_mem;
		table->cols = cols;
		table->rows = rows;
		table->table = ((wchar_t **)(table_mem + table_offset));
		table->width = ((size_t *)(table_mem + width_offset));
		table->title = NULL;
		table->write = write;
		table->user = NULL;
		table->limit = 0;
	} else {
		return NULL;
	}
	
	for (size_t i = 0, j = cols * (rows + 1); i < j; i++) {
		table->table[i] = NULL;
	}
	
	for (size_t i = 0; i < cols; i++) {
		table->width[i] = 0;
	}
	
	return table;
}

inline static uint8_t
within(const table_t *table, size_t col, size_t row)
{
	return table ? table->cols > col && table->rows > row : 0;
}

void
table_set_cell(table_t *table, size_t col, size_t row, wchar_t *value)
{
	if (within(table, col, row)) {
		CELL(table, col, row) = value;
	}
}

static void
set_row(table_t *table, size_t row, va_list args)
{
	if (table) {
		wchar_t *value;

		for (size_t col = 0, cols = table->cols; col < cols; col++) {
			value = va_arg(args, wchar_t *);
			table_set_cell(table, col, row, value);
		}
	}
}

void
table_set_row(table_t *table, size_t row, ...)
{
	if (table) {
		va_list args;
		
		va_start(args, row);
		set_row(table, row, args);
		va_end(args);
	}
}

void
table_set_header(table_t *table, ...)
{
	if (table) {
		va_list args;
		
		va_start(args, table);
		set_row(table, table->rows++, args);
		va_end(args);
		
		table->rows--;
	}
}

void
table_set_title(table_t *table, wchar_t *title)
{
	if (table) {
		table->title = title;
	}
}

inline static wchar_t
*print_fill(wchar_t *s, size_t n, wchar_t c)
{
	for (wchar_t *p = s + n; s < p; *s++ = c);
	return s;
}

static wchar_t
*print_cell(const table_t *table, wchar_t *s,  size_t col, size_t row)
{	
	wchar_t *value = CELL(table, col, row);
	size_t len = value ? wcsnlen(value, table->width[col]) : 0;
	
	*s++ = L'\u2502';
	*s++ = L' ';	
	wmemcpy(s, value, len);
	s += len;
	s = print_fill(s, table->width[col] - len + 1, ' ');

	if (col == table->cols - 1) {
		*s++ = L'\u2502';
	}

	*s = L'\0';
	return s;
}

inline static void
print_row(const table_t *table, wchar_t *s, size_t row)
{
	wchar_t *buf = s;

	for (size_t col = 0, cols = table->cols; col < cols; col++) {
		s = print_cell(table, s, col, row);
	}
	
	*s = '\0';
	table->write(buf, s - buf, table->user);
	s = buf;
}

inline static void
set_col_div(const table_t *table, wchar_t *s, wchar_t c)
{
	for (size_t col = 0, pos = 0; col < table->cols - 1; col++) {
		pos += 2 + table->width[col] + 1;
		s[pos] = c;
	}
}

typedef enum {
	SEP_TITLE_TOP,
	SEP_TITLE_BOTTOM,
	SEP_HEADER_TOP,
	SEP_HEADER_BOTTOM,
	SEP_BOTTOM
} sep_t;

static void
print_separator(const table_t *table, wchar_t *s, sep_t sep)
{
	wchar_t *first = s;
	wchar_t *last = s + table->line_width - 1;
	
	wmemset(s, L'\u2500', table->line_width);

	switch (sep) {
	case SEP_TITLE_TOP:
		*first = L'\u250C';
		*last = L'\u2510';
		break;
	case SEP_TITLE_BOTTOM:
		*first = L'\u251C';
		*last = L'\u2524';
		set_col_div(table, s, L'\u252C');
		break;
	case SEP_HEADER_TOP:
		*first = L'\u250C';
		*last = L'\u2510';
		set_col_div(table, s, L'\u252C');
		break;
	case SEP_HEADER_BOTTOM:
		*first = L'\u251C';
		*last = L'\u2524';
		set_col_div(table, s, L'\u253C');
		break;
	case SEP_BOTTOM:
		*first = L'\u2514';
		*last = L'\u2518';
		set_col_div(table, s, L'\u2534');
		break;
	}
	
	*(last + 1) = L'\0';
	table->write(first, table->line_width, table->user);
}

static void
print_title(const table_t *table, wchar_t *s)
{
	if (table->title) {
		print_separator(table, s, SEP_TITLE_TOP);
		
		wchar_t *buf = s;
		size_t title_len = wcslen(table->title);
		size_t offset = (table->line_width / 2) - (title_len / 2);

		wmemset(s, L' ', table->line_width);
		wmemcpy(s + offset, table->title, title_len);

		*s = *(s + table->line_width - 1) = L'\u2502';
		*(s + table->line_width) = L'\0';
		table->write(buf, s - buf, table->user);
		print_separator(table, s, SEP_TITLE_BOTTOM);
	} else {
		print_separator(table, s, SEP_HEADER_TOP);
	}
}

static void
print_header(table_t *table, wchar_t *s)
{
	print_title(table, s);
	size_t row = table->rows++;
	print_row(table, s, row);
	print_separator(table, s, SEP_HEADER_BOTTOM);
	table->rows--;
}

void
table_print(table_t *table)
{
	if (!table || !table->write) {
		return;
	}

	/* Iterate all rows including the headers. */
	wchar_t *value = NULL;
	for (size_t row = 0, rows = table->rows + 1; row < rows; row++) {
		/* Compute the width of the widest columns. */
		for (size_t col = 0, len = 0; col < table->cols; col++) {
			value = CELL(table, col, row);
			len = value ? wcslen(value) : 0;
			table->width[col] = util_max(table->width[col], len);
		}
	}

	/* Compute maximum line width. */
	table->line_width = (3 * table->cols) + 1;
	for (size_t col = 0; col < table->cols; col++) {
		table->line_width += table->width[col];
	}
	
	/* Write table, line by line. */
	wchar_t *buf = malloc(sizeof(*buf) * (table->line_width + 1));
	print_header(table, buf);
	size_t limit = util_min(table->limit, table->rows);
	limit = limit ? limit : table->rows;
	for (size_t row = 0; row < limit; row++) {
		print_row(table, buf, row);
	}
	
	print_separator(table, buf, SEP_BOTTOM);
	free(buf);
}

#undef CELL

