#include "lexer.h"
#include "test_util.h"

void TestNextToken(void) {
	char* input = "=+(){},;";

	struct TestNextToken_ExpectedResults {
		tokentype expectedType;
	       	char* expectedLiteral;
	};

	static struct TestNextToken_ExpectedResults tests[] = {
		{TOK_ASSIGN, "="},
		{TOK_PLUS, "+"},
		{TOK_LPAREN, "("},
		{TOK_RPAREN, ")"},
		{TOK_LBRACE, "{"},
		{TOK_RBRACE, "}"},
		{TOK_COMMA, ","},
		{TOK_SEMICOLON, ";"},
		{TOK_EOF, ""},
		{0, NULL}
	};

	lexer* l = NewLexer(input, strlen(input));

	for (int i = 0 ; tests[i].expectedLiteral != NULL ; i++) {
		token* t = NextToken(l);

		if (t->type != tests[i].expectedType) {
			fail("Test %d, token type '%s' should be '%s'\n", i,
				tokentype2str(t->type),
				tokentype2str(tests[i].expectedType));
		}

		str_should_equal(t->literal, tests[i].expectedLiteral);

		FreeToken(t);
	}

	FreeLexer(l);

}

int main(void) {
	TestNextToken();
}
