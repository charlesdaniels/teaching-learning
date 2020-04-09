#include "tokens.h"

token* NewToken(tokentype type, char* literal) {
	token* t = malloc(sizeof(token));
	t->type = type;
	t->literal = strdup(literal);
	return t;
}

token* NewTokenFromChar(tokentype type, char literal) {
	token* t;
	char* literal_str;
	asprintf(&literal_str, "%c", literal);
	t = NewToken(type, literal_str);
	free(literal_str); /* duplicated in NewToken */
	return t;
}

void FreeToken(token* t) {
	free(t->literal);
	free(t);
}

tokentype LookupIdent(char* literal) {
	if (strcmp(literal, "fn") == 0) { return TOK_FUNCTION; }
	if (strcmp(literal, "let") == 0) { return TOK_LET; }

	return TOK_IDENT; 
}
