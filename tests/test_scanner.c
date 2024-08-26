#include "map/map_parser.h"
#include "map/map_scanner.h"
#include "common.h"

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
	const yytoken_kind_t expected_tokens[] = {
		'{', TOKEN_STRING, TOKEN_STRING, TOKEN_STRING, TOKEN_STRING, TOKEN_STRING, TOKEN_STRING, TOKEN_STRING, TOKEN_STRING, '{', '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', TOKEN_TEXTURE, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', TOKEN_TEXTURE, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', TOKEN_TEXTURE, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', TOKEN_TEXTURE, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', TOKEN_TEXTURE, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', TOKEN_TEXTURE, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, '}', '}', '{', TOKEN_STRING, TOKEN_STRING, TOKEN_STRING, TOKEN_STRING, '}'};
	const char *expected_lexemes[] = {
		"{", "\"sounds\"", "\"1\"", "\"classname\"", "\"worldspawn\"", "\"wad\"", "\"/gfx/base.wad\"", "\"worldtype\"", "\"0\"", "{", "(", "-128", "0", "0", ")", "(", "128.2", "-1.0", "0", ")", "(", "128", "0", "1", ")", "GROUND1_6", "0", "0", "0", "1.0", "1.0", "(", "256", "0", "0", ")", "(", "256", "0", "1", ")", "(", "256", "1", "0", ")", "GROUND1_6", "0", "0", "0", "1.0", "1.0", "(", "0", "128", "0", ")", "(", "0", "128", "1", ")", "(", "1", "128", "0", ")", "GROUND1_6", "0", "0", "0", "1.0", "1.0", "(", "0", "384", "0", ")", "(", "1", "384", "0", ")", "(", "0", "384", "1", ")", "GROUND1_6", "0", "0", "0", "1.0", "1.0", "(", "0", "0", "64", ")", "(", "1", "0", "64", ")", "(", "0", "1", "64", ")", "GROUND1_6", "0", "0", "0", "1.0", "1.0", "(", "0", "0", "128", ")", "(", "0", "1", "128", ")", "(", "1", "0", "128", ")", "GROUND1_6", "0", "0", "0", "1.0", "1.0", "}", "}", "{", "\"classname\"", "\"info_player_start\"", "\"origin\"", "\"256 384 160\"", "}"};


	int succeed = 0;

	YY_BUFFER_STATE buf = yy_scan_string(source);

	yytoken_kind_t tok = yylex();

	size_t i = 0;
	const size_t tok_cnt = sizeof(expected_tokens) / sizeof(yytoken_kind_t);

	for (;i < tok_cnt && tok != YYEOF; i++, tok = yylex()) {
		if (tok == YYerror || tok == YYUNDEF) {
			PRINT_ERR("token %lu: unknown token %s\n", i, yytext);
			succeed = -1;
		}
		if (expected_tokens[i] != tok) {
			PRINT_ERR("token %lu: expected token %d, got %d\n",
			      i, expected_tokens[i], tok);
			succeed = -1;
		}
		if (strcmp(expected_lexemes[i], yytext) != 0) {
			PRINT_ERR("token %lu: expected lexeme %s, got %s\n",
			      i, expected_lexemes[i], yytext);
			succeed = -1;
		}
	}

	if (tok != 0) {
		PRINT_ERR("didn't get EOF past end of input\n");
		succeed = -1;
	}

	if (i < tok_cnt) {
		PRINT_ERR("Received EOF early at token %lu: %s\n", i, yytext);
		succeed = -1;
	}

	yy_delete_buffer(buf);

	return succeed;
}
