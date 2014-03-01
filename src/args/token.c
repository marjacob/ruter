#include <stdbool.h>
#include <string.h>
#include "args/csrc.h"
#include "args/token.h"

inline static bool is_sep(char c)
{
	switch (c) {
	case ' ':
	case '\t':
	case '\0':
		return true;
	}
	
	return false;
}

inline static void tok_append(tok_t *dest, tok_t *src)
{
	size_t a = strlen(dest->text);
	size_t b = strlen(src->text);
	
	dest->text = realloc(dest->text, a + b + 2);
	dest->text[a] = ' ';
	strncpy(dest->text + a + 1, src->text, b + 1);
}

inline static bool tok_both(tok_t *t1, tok_t *t2, tok_e type)
{
	return (type == t1->type && t1->type == t2->type);
}

static char *next_word(csrc_t *src)
{
	size_t i = 1;
	char *word = NULL;
	
	for (char c = csrc_next(src); !is_sep(c); c = csrc_next(src)) {
		word = realloc(word, ++i);
		word[i - 2] = c;
		word[i - 1] = '\0';
	}
	
	return word;
}

tok_t *tok_next(csrc_t *src)
{
	char *word = next_word(src);
	
	if (!word) {
		return NULL;
	}
	
	tok_t *tok = malloc(sizeof(*tok));
	
	if (!strcmp("find", word)) {
		tok->type = TOK_FIND;
	} else if (!strcmp("show", word)) {
		tok->type = TOK_SHOW;
	} else if (!strcmp("from", word)) {
		tok->type = TOK_FROM;
	} else if (!strcmp("to", word)) {
		tok->type = TOK_TO;
	} else {
		tok->type = TOK_TEXT;
		tok->text = word;
	}
	
	if (TOK_TEXT != tok->type) {
		tok->text = NULL;
		free(word);
	}
	
	tok->next = NULL;
	tok->prev = NULL;
	
	return tok;
}

tok_t *tok_tokenize(csrc_t *src)
{
	tok_t *head = NULL;
		
	for (tok_t *cur = tok_next(src), *tail; cur; cur = tok_next(src)) {
		if (!head) {
			head = tail = cur;
		} else if (tok_both(tail, cur, TOK_TEXT)) {
			tok_append(tail, cur);
			tok_free(cur);
		} else {
			cur->prev = tail;
			tail = tail->next = cur;
		}
	}
	
	return head;
}

void tok_free(tok_t *tok)
{
	if (tok) {
		tok_free(tok->next);
		free(tok->text);
		free(tok);
	}
}

