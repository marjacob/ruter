#include <stdbool.h>
#include <stdlib.h>
#include "args/args.h"
#include "args/csrc.h"
#include "args/token.h"
#include "util.h"

#include <wchar.h>

static args_t args_zero = { 0 };

args_t *args_parse(size_t argc, char **argv)
{
	csrc_t *src = csrc_init(argc, argv);
	tok_t *tokens = tok_tokenize(src);
	
	tok_t *prev = NULL;
	args_t *args = malloc(sizeof(*args));
	*args = args_zero;
	
	for (tok_t *t = tokens; t; t = t->next) {
		switch (!prev ? TOK_NONE : prev->type) {
		case TOK_NONE:
			break;
		case TOK_TEXT:
			if (TOK_TO == t->type) {
				args->from = util_strdup(prev->text);
			} else if (TOK_FROM == t->type) {
				args->to = util_strdup(prev->text);
			}
			break;
		case TOK_FROM:
			if (TOK_TEXT == t->type) {
				args->from = util_strdup(t->text);
			}
			break;
		case TOK_TO:
			if (TOK_TEXT == t->type) {
				args->to = util_strdup(t->text);
			}
			break;
		case TOK_FIND:
			if (TOK_TEXT == t->type) {
				args->find = util_strdup(t->text);
			}
			break;
		case TOK_SHOW:
			if (TOK_TEXT == t->type) {
				args->show = util_strdup(t->text);
			}
			break;
		}
		
		prev = t;
	}
	
	tok_free(tokens);
	csrc_free(src);
	
	return args;
}

void args_free(args_t *args)
{
	free(args->find);
	free(args->from);
	free(args->show);
	free(args->to);
	free(args);
}
