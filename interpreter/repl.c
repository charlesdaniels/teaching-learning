#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "lexer.h"
#include "tokens.h"


int main(void) {
	char* buf;
	lexer* l;

	while ((buf = readline(">> ")) != NULL) {
		if (strlen(buf) > 0) {
			add_history(buf);
		}

		l = NewLexer(buf, strlen(buf));

		for (token* t = NextToken(l) ; t->type != TOK_EOF ; t = NextToken(l)) {
			printf("{Type:%s Literal:%s}\n",
					tokentype2str(t->type), t->literal);
			FreeToken(t);
		}
		
		FreeLexer(l);
		free(buf);
	}

	printf("\nexiting interpreter.\n\n");


	return 0;
}
