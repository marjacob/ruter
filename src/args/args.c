#include <stdlib.h>
#include "args/args.h"
#include "args/csrc.h"
#include "args/token.h"
#include "util.h"

static struct args args_zero = { 0 };

int args_parse(struct args *args, size_t argc, char **argv)
{
	csrc_t *src = csrc_init(argc, argv);
	tok_t *tokens = tok_tokenize(src);
	tok_t *prev = NULL;
	*args = args_zero;
	
	for (tok_t *t = tokens; t; t = t->next) {
		switch (!prev ? TOK_NONE : prev->type) {
		case TOK_NONE:
			break;
		case TOK_TEXT:
			if (TOK_TO == t->type) {
				free(args->from);
				args->from = util_strdup(prev->text);
			} else if (TOK_FROM == t->type) {
				free(args->to);
				args->to = util_strdup(prev->text);
			}
			break;
		case TOK_FROM:
			if (TOK_TEXT == t->type) {
				free(args->from);
				args->from = util_strdup(t->text);
			}
			break;
		case TOK_TO:
			if (TOK_TEXT == t->type) {
				free(args->to);
				args->to = util_strdup(t->text);
			}
			break;
		case TOK_FIND:
			if (TOK_TEXT == t->type) {
				free(args->find);
				args->find = util_strdup(t->text);
			}
			break;
		case TOK_SHOW:
			if (TOK_TEXT == t->type) {
				free(args->show);
				args->show = util_strdup(t->text);
			}
			break;
		}
		
		prev = t;
	}
	
	tok_free(tokens);
	csrc_free(src);
	
	return 1;
}

void args_free(struct args *args)
{
	free(args->find);
	free(args->from);
	free(args->show);
	free(args->to);
}

