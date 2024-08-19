#include "map_scanner.h"
#include "common.h"
#include "string.h"

int main() {
	const char *source = "{"
		" \"sounds\" \"1\""
		" \"classname\" \"worldspawn\""
		" \"wad\" \"/gfx/base.wad\""
		" \"worldtype\" \"0\""
		" {"
		"  ( -128 0 0 ) ( 128.2 -1.0 0 ) ( 128 0 1 ) GROUND1_6 0 0 0 1.0 1.0"
		"  ( 256 0 0 ) ( 256 0 1 ) ( 256 1 0 ) GROUND1_6 0 0 0 1.0 1.0"
		"  ( 0 128 0 ) ( 0 128 1 ) ( 1 128 0 ) GROUND1_6 0 0 0 1.0 1.0"
		"  ( 0 384 0 ) ( 1 384 0 ) ( 0 384 1 ) GROUND1_6 0 0 0 1.0 1.0"
		"  ( 0 0 64 ) ( 1 0 64 ) ( 0 1 64 ) GROUND1_6 0 0 0 1.0 1.0"
		"  ( 0 0 128 ) ( 0 1 128 ) ( 1 0 128 ) GROUND1_6 0 0 0 1.0 1.0"
		" }"
		"}"
		"{"
		" \"classname\" \"info_player_start\""
		" \"origin\" \"256 384 160\""
		"}";
	const Token expected_tokens[] = {
		'{', TOKEN_STRING, TOKEN_STRING, TOKEN_STRING, TOKEN_STRING, TOKEN_STRING, TOKEN_STRING, TOKEN_STRING, TOKEN_STRING, '{', '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', TOKEN_TEXTURE, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', TOKEN_TEXTURE, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', TOKEN_TEXTURE, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', TOKEN_TEXTURE, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', TOKEN_TEXTURE, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', TOKEN_TEXTURE, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, '}', '}', '{', TOKEN_STRING, TOKEN_STRING, TOKEN_STRING, TOKEN_STRING, '}'};
const char *expected_lexemes[] = {
	"{", "\"sounds\"", "\"1\"", "\"classname\"", "\"worldspawn\"", "\"wad\"", "\"/gfx/base.wad\"", "\"worldtype\"", "\"0\"", "{", "(", "-128", "0", "0", ")", "(", "128.2", "-1.0", "0", ")", "(", "128", "0", "1", ")", "GROUND1_6", "0", "0", "0", "1.0", "1.0", "(", "256", "0", "0", ")", "(", "256", "0", "1", ")", "(", "256", "1", "0", ")", "GROUND1_6", "0", "0", "0", "1.0", "1.0", "(", "0", "128", "0", ")", "(", "0", "128", "1", ")", "(", "1", "128", "0", ")", "GROUND1_6", "0", "0", "0", "1.0", "1.0", "(", "0", "384", "0", ")", "(", "1", "384", "0", ")", "(", "0", "384", "1", ")", "GROUND1_6", "0", "0", "0", "1.0", "1.0", "(", "0", "0", "64", ")", "(", "1", "0", "64", ")", "(", "0", "1", "64", ")", "GROUND1_6", "0", "0", "0", "1.0", "1.0", "(", "0", "0", "128", ")", "(", "0", "1", "128", ")", "(", "1", "0", "128", ")", "GROUND1_6", "0", "0", "0", "1.0", "1.0", "}", "}", "{", "\"classname\"", "\"info_player_start\"", "\"origin\"", "\"256 384 160\"", "}"};

	YY_BUFFER_STATE buf = yy_scan_string(source);
	int succeed = 0;
	Token tok = yylex();
	size_t i = 0;
	const size_t token_count = sizeof(expected_tokens) / sizeof(Token);

	for (;i < token_count && tok != TOKEN_EOF; i++, tok = yylex()) {
		if (tok == TOKEN_ERROR) {
			ERROR("token %lu: unknown token %s\n", i, yytext);
			succeed = -1;
		}
		if (expected_tokens[i] != tok) {
			ERROR("token %lu: expected token %d, got %d\n",
			      i, expected_tokens[i], tok);
			succeed = -1;
		}
		if (strcmp(expected_lexemes[i], yytext) != 0) {
			ERROR("token %lu: expected lexeme %s, got %s\n",
			      i, expected_lexemes[i], yytext);
			succeed = -1;
		}
	}

	if (tok != TOKEN_EOF) {
		ERROR("didn't get EOF past end of input\n");
		succeed = -1;
	}

	if (i < token_count) {
		ERROR("Received EOF early at token %lu: %s\n", i, yytext);
		succeed = -1;
	}

	yy_delete_buffer(buf);

	return succeed;
}
