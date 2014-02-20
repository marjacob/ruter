#include <stdbool.h>
#include <stdlib.h>
#include "args/args.h"
#include "util.h"

static args_t args_zero = { 0 };

args_t *args_parse(tok_t *tokens)
{
	tok_e prev = TOK_NONE;
	args_t *args = malloc(sizeof(*args));
	*args = args_zero;
	
	for (tok_t *t = tokens; t; t = t->next) {
		switch (prev) {
		case TOK_NONE:
		case TOK_TEXT:
			break;
		case TOK_FIND:
			if (TOK_TEXT != t->type) {
				return NULL;
			}
			
			args->find = util_strdup(t->text);
			break;
		case TOK_SHOW:
			if (TOK_TEXT != t->type) {
				return NULL;
			}
			
			args->show = util_strdup(t->text);
			break;
		}
		
		prev = t->type;
	}
	
	return args;
}

void args_free(args_t *args)
{
	free(args->find);
	free(args->show);
	free(args);
}
