#ifndef __H_TABLE__
#define __H_TABLE__

#include <inttypes.h>
#include <stdlib.h>

/* The box drawing characters used are documented on Wikipedia:
 * http://en.wikipedia.org/wiki/Box-drawing_character#Unicode */

typedef void
(*writelfunc)(wchar_t *ptr, size_t len, void *user);

/**
 * table_t - Describes a table.
 *
 * @cols	Total number of columns.
 * @rows	Total number of rows.
 * @limit	Maximum number of rows to be printed.
 * @line_width	Maximum line width.
 * @width	Array indexed by column giving its maximum width.
 * @table	Underlying table array.
 * @title	Pointer to table title.
 * @user	Pointer to user memory, passed to the write() function.
 * @write	Pointer to write function.
 *
 * This structure describes the various features of a table. Its values should
 * not be accessed directly. Use the table_*-functions to manipulate them.
 */
typedef struct {
	size_t cols;
	size_t rows;
	size_t limit;
	size_t line_width;
	size_t *width;
	wchar_t **table;
	wchar_t *title;
	void *user;
	writelfunc write;
} table_t;

/**
 * table_create() - Creates a table structure.
 *
 * @cols	Number of columns.
 * @rows	Number of rows.
 * @write	Pointer to line write function.
 *
 * This function allocates memory for a table structure. The write function
 * is called by table_print() for every line it generates. The simplest write
 * function is one that just calls wprintf() on the string it receives.
 *
 * Function signature for the write function:
 *     void write(wchar_t *ptr, size_t len, void *user);
 * The 'ptr' parameter points to the start of the line, which is of length 
 * 'len'. The 'user' parameter points to whatever you want. Take a look at the
 * table_set_user() function.
 *
 * Returns: Pointer to table structure.
 */
table_t
*table_create(size_t cols, size_t rows, writelfunc write);

/**
 * table_free() - Frees the memory associated with a table structure.
 *
 * @table	Pointer to table structure.
 *
 * This function frees all memory that has been allocated for the table
 * structure and its associated buffers.
 */
inline static void
table_free(table_t *table)
{
	if (table) {
		free(table);
	}
}

/**
 * table_len() - Computes total length of wide character table representation.
 * 
 * @table	Pointer to table structure.
 *
 * This function computes the number of characters needed to store the entire
 * table in a single buffer. Space for '\n' characters at the end of each line
 * is considered, but the terminating '\0' character is not.
 */
inline static size_t
table_len(const table_t *table)
{
	if (table) {
		size_t rows = table->rows + 4 + table->title ? 2 : 0;
		return (table->line_width + 1) * rows;
	}
}

/**
 * table_set_limit() - Specifies the number of rows to be printed by calling
 *                     the table_print() function.
 *
 * @table	Pointer to table structure.
 * @limit	Maximum number of rows to print.
 *
 * This function specifies the maximum number of rows to print during a call
 * to table_print(). To print all available rows, set the limit to zero.
 */
inline static void
table_set_limit(table_t *table, size_t limit)
{
	if (table) {
		table->limit = limit;
	}
}

/**
 * table_set_user() - Specifies the 'user' pointer used by the write function.
 *
 * @table	Pointer to table structure.
 * @user	Pointer to user memory.
 *
 * This function specifies the pointer passed with the 'user' parameter during
 * a call to the write function.
 */
inline static void
table_set_user(table_t *table, void *user)
{
	if (table) {
		table->user = user;
	}
}

/**
 * table_set_cell() - Sets the content of a cell in the table.
 *
 * @table	Pointer to table structure.
 * @col		Column number.
 * @row		Row number.
 * @value	Pointer to value.
 *
 * This function sets the content of a cell in the table, specified with a
 * column and row number. The table itself, is a table of pointers to wide
 * character strings. Do not call free() on any value stored in this table
 * before the table itself has been freed with table_free().
 */
void
table_set_cell(table_t *table, size_t col, size_t row, wchar_t *value);

/**
 * table_set_row() - Sets all the columns in a row.
 *
 * @table	Pointer to table structure.
 * @row		Row number.
 * @...		List of pointers.
 *
 * This function sets all the columns for a given row number. As such, the
 * number of arguments must match the number of columns to prevent undefined
 * behaviour. Any NULL pointers will be printed as empty cells.
 */
void
table_set_row(table_t *table, size_t row, ...);

/**
 * table_set_header() - Sets all the headers.
 *
 * @table	Pointer to table structure.
 * @...		List of pointers.
 *
 * This function sets all the column headers for the table. As such, the
 * number of arguments must match the number of columns to prevent undefined
 * behaviour. Any NULL pointers will be printed as empty cells.
 */
void
table_set_header(table_t *table, ...);

/**
 * table_set_title() - Sets the title of a table.
 *
 * @table	Pointer to table structure.
 * @title	Pointer to title.
 * 
 * This function sets the title of a table. The title defaults to NULL, which
 * causes no title to be printed by table_print().
 */
void
table_set_title(table_t *table, wchar_t *title);

/**
 * table_print() - Generates and prints the table.
 *
 * @table	Pointer to table structure.
 *
 * This function generates a wide character representation of the table,
 * line by line. For each generated line it calls the write() function as
 * specified to table_create().
 */
void
table_print(table_t *table);

#endif

