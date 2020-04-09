#ifndef TOKENS_H
#define TOKENS_H

#define _GNU_SOURCE
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef enum {
	TOK_ILLEGAL,
	TOK_EOF,
	TOK_IDENT,
	TOK_INT,
	TOK_ASSIGN,
	TOK_PLUS,
	TOK_COMMA,
	TOK_SEMICOLON,
	TOK_LPAREN,
	TOK_RPAREN,
	TOK_LBRACE,
	TOK_RBRACE,
	TOK_FUNCTION,
	TOK_LET
} tokentype;

typedef struct{
	tokentype type;
	char* literal;
} token;

#define tokentype2str(_t) \
	(_t == TOK_ILLEGAL) ? "ILLEGAL" : \
	(_t == TOK_EOF) ? "EOF" : \
	(_t == TOK_IDENT) ? "IDENT" : \
	(_t == TOK_INT) ? "INT" : \
	(_t == TOK_ASSIGN) ? "=" : \
	(_t == TOK_PLUS) ? "+" : \
	(_t == TOK_COMMA) ? "," : \
	(_t == TOK_SEMICOLON) ? ";" : \
	(_t == TOK_LPAREN) ? "(" : \
	(_t == TOK_RPAREN) ? ")" : \
	(_t == TOK_LBRACE) ? "{" : \
	(_t == TOK_RBRACE) ? "}" : \
	(_t == TOK_FUNCTION) ? "FUNCTION" : \
	(_t == TOK_LET) ? "LET" :  "UNKNOWN TOKEN"

/**
 * @brief Allocate a new token.
 *
 * NOTE: the literal is duplicated by this function and free-ed by FreeToken.
 *
 * @param t
 * @param literal
 *
 * @return
 */
token* NewToken(tokentype type, char* literal);

/**
 * @brief Works similarly to NewToken, but handles the case where literal is of
 * type char.
 *
 * @param type
 * @param literal
 *
 * @return
 */
token* NewTokenFromChar(tokentype type, char literal);

/**
 * @brief Free a token including it's copy of the literal.
 *
 * @param t
 */
void FreeToken(token* t);

#endif /* TOKENS_H */
