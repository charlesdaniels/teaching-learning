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
