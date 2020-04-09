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

token* readIdentifier(lexer* l) {
	char* ident_string;
	size_t position;
	token * t;
	
	position = l->position;
	while (isLetter(l->ch)) {
		readChar(l);
	}

	ident_string = strndup((l->input + position), (l->position - position));
	t = NewToken(LookupIdent(ident_string), ident_string);
	free(ident_string);

	return t;
}

void skipWhitespace(lexer* l) {
	while (isWhitespace(l->ch)) {
		readChar(l);
	}
}

token* readNumber(lexer* l) {
	char* num_string;
	size_t position;
	token * t;
	
	position = l->position;
	while (isDigit(l->ch)) {
		readChar(l);
	}

	num_string = strndup((l->input + position), (l->position - position));
	t = NewToken(TOK_INT, num_string);
	free(num_string);

	return t;
}

token* NextToken(lexer* l) {
	token* t = NULL;

	/* macro to handle single-character tokens */
#define singlechar(_compchar, _tokentype) \
	else if (_compchar == l->ch) { t = NewTokenFromChar(_tokentype, l->ch); }

	skipWhitespace(l);

	if (false) { /* do nothing */ }
	singlechar('=', TOK_ASSIGN)
	singlechar(';', TOK_SEMICOLON)
	singlechar('(', TOK_LPAREN)
	singlechar(')', TOK_RPAREN)
	singlechar(',', TOK_COMMA)
	singlechar('+', TOK_PLUS)
	singlechar('{', TOK_LBRACE)
	singlechar('}', TOK_RBRACE)
	singlechar('\0', TOK_EOF)
	else {
		if (isLetter(l->ch)) {
			t = readIdentifier(l);

			/* avoid calling readChar() again, since we already
			 * called it inside of readIdentifier */
			return t;

		} else if (isDigit(l->ch)) {
			t = readNumber(l);
			return t;

		} else {
			t = NewTokenFromChar(TOK_ILLEGAL, l->ch);
		}
	}

	readChar(l);

	return t;

#undef singlechar

}
