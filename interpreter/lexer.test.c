#include "lexer.h"
#include "test_util.h"

struct TestNextToken_ExpectedResults {
	tokentype expectedType;
	char* expectedLiteral;
};

void TestNextToken_01(void) {
	char* input = "=+(){},;";


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

void TestNextToken_02(void) {
	char* input =
		"let five = 5;\n"
		"let ten = 10;\n"
		"\n"
		"let add = fn(x, y) {\n"
		"    x + y;\n"
		"};\n"
		"\n"
		"let result = add(five, ten);";


	struct TestNextToken_ExpectedResults {
		tokentype expectedType;
	       	char* expectedLiteral;
	};

	static struct TestNextToken_ExpectedResults tests[] = {
		{TOK_LET, "let"},
		{TOK_IDENT, "five"},
		{TOK_ASSIGN, "="},
		{TOK_INT, "5"},
		{TOK_SEMICOLON, ";"},
		{TOK_LET, "let"},
		{TOK_IDENT, "ten"},
		{TOK_ASSIGN, "="},
		{TOK_INT, "10"},
		{TOK_SEMICOLON, ";"},
		{TOK_LET, "let"},
		{TOK_IDENT, "add"},
		{TOK_ASSIGN, "="},
		{TOK_FUNCTION, "fn"},
		{TOK_LPAREN, "("},
		{TOK_IDENT, "x"},
		{TOK_COMMA, ","},
		{TOK_IDENT, "y"},
		{TOK_RPAREN, ")"},
		{TOK_LBRACE, "{"},
		{TOK_IDENT, "x"},
		{TOK_PLUS, "+"},
		{TOK_IDENT, "y"},
		{TOK_SEMICOLON, ";"},
		{TOK_RBRACE, "}"},
		{TOK_SEMICOLON, ";"},
		{TOK_LET, "let"},
		{TOK_IDENT, "result"},
		{TOK_ASSIGN, "="},
		{TOK_IDENT, "add"},
		{TOK_LPAREN, "("},
		{TOK_IDENT, "five"},
		{TOK_COMMA, ","},
		{TOK_IDENT, "ten"},
		{TOK_RPAREN, ")"},
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
	TestNextToken_01();
	TestNextToken_02();
}
