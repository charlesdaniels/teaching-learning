#include "lexer.h"

lexer* NewLexer(char* input, size_t inputLength) {
	lexer* l = malloc(sizeof(lexer));

	l->input = input;
	l->position = 0;
	l->readPosition = 0;
	l->inputLength = inputLength;
	l->ch = '\0';

	/* The first token will implicitly be an EOF, which we don't want, so
	 * we go ahead and flush it now. */
	FreeToken(NextToken(l));

	return l;

}

void FreeLexer(lexer* l) {
	free(l);
}

void readChar(lexer* l) {
	if (l->readPosition >= l->inputLength){
		l->ch = '\0';
	} else {
		l->ch = l->input[l->readPosition];
	}

	l->position = l->readPosition;
	l->readPosition ++;
}

token* NextToken(lexer* l) {
	token* t;

	/* macro to handle single-character tokens */
#define singlechar(_compchar, _tokentype) \
	else if (_compchar == l->ch) { t = NewTokenFromChar(_tokentype, l->ch); }

	if (false) { /* do nothing */ }
	singlechar('=', TOK_ASSIGN)
	singlechar(';', TOK_SEMICOLON)
	singlechar('(', TOK_LPAREN)
	singlechar(')', TOK_RPAREN)
	singlechar(',', TOK_COMMA)
	singlechar('+', TOK_PLUS)
	singlechar('{', TOK_LBRACE)
	singlechar('}', TOK_RBRACE)
	else {
		t = NewToken(TOK_EOF, "");
	}

	readChar(l);

	return t;

#undef singlechar

}
