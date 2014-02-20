#ifndef __H_TOKEN__
#define __H_TOKEN__

#include "args/csrc.h"

typedef enum {
	TOK_SHOW,
	TOK_FIND,
	TOK_TEXT,
	TOK_NONE
} tok_e;

typedef struct tok {
	tok_e type;
	char *text;
	struct tok *next;
	struct tok *prev;
} tok_t;

tok_t *tok_next(csrc_t *src);

tok_t *tok_tokenize(csrc_t *src);

void tok_free(tok_t *tok);

#endif
