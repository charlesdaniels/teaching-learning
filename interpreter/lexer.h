#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "tokens.h"
#include "util.h"

typedef struct {
	char* input;
	size_t position;
	size_t readPosition;
	size_t inputLength;
	char ch;		/* NULL signifies EOF or nothing read yet */
} lexer;

/**
 * @brief Allocate a new lexer struct.
 *
 * The caller is responsible for allocating the input string, and for free-ing
 * the lexer using FreeLexer().
 *
 * The structure fields will be initialized to default values.
 *
 * @param input The input string.
 * @param inputLength The length of the input string.
 *
 * @return The newly allocated lexer struct.
 */
lexer* NewLexer(char* input, size_t inputLength);

/**
 * @brief De-allocate an existing lexer.
 *
 * NOTE: the lexer's input string is not free-ed by this function, it should be
 * free-ed by the caller if appropriate.
 *
 * @param l The lexer to de-allocate.
 */
void FreeLexer(lexer* l);

/**
 * @brief Advance the cursor position by one.
 *
 * @param l
 */
void readChar(lexer* l);

/**
 * @brief Read the next token from the lexer.
 *
 * @param l
 *
 * @return Newly allocated token object which the caller must later free using
 * FreeToken().
 */
token* NextToken(lexer* l);

#endif /* LEXER_H */
