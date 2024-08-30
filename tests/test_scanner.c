#include "map/map.h"
#include "map/map_parser.h"
#include "map/map_scanner.h"
#include <assert.h>

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
	const yytoken_kind_t expectedTokens[] = {
		'{', TOKEN_STRING, TOKEN_STRING, TOKEN_STRING, TOKEN_STRING, TOKEN_STRING, TOKEN_STRING, TOKEN_STRING, TOKEN_STRING, '{', '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', TOKEN_TEXTURE, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', TOKEN_TEXTURE, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', TOKEN_TEXTURE, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', TOKEN_TEXTURE, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', TOKEN_TEXTURE, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', '(', TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, ')', TOKEN_TEXTURE, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, TOKEN_NUMBER, '}', '}', '{', TOKEN_STRING, TOKEN_STRING, TOKEN_STRING, TOKEN_STRING, '}'};
	const char *expectedLexemes[] = {
		"{", "\"sounds\"", "\"1\"", "\"classname\"", "\"worldspawn\"", "\"wad\"", "\"/gfx/base.wad\"", "\"worldtype\"", "\"0\"", "{", "(", "-128", "0", "0", ")", "(", "128.2", "-1.0", "0", ")", "(", "128", "0", "1", ")", "GROUND1_6", "0", "0", "0", "1.0", "1.0", "(", "256", "0", "0", ")", "(", "256", "0", "1", ")", "(", "256", "1", "0", ")", "GROUND1_6", "0", "0", "0", "1.0", "1.0", "(", "0", "128", "0", ")", "(", "0", "128", "1", ")", "(", "1", "128", "0", ")", "GROUND1_6", "0", "0", "0", "1.0", "1.0", "(", "0", "384", "0", ")", "(", "1", "384", "0", ")", "(", "0", "384", "1", ")", "GROUND1_6", "0", "0", "0", "1.0", "1.0", "(", "0", "0", "64", ")", "(", "1", "0", "64", ")", "(", "0", "1", "64", ")", "GROUND1_6", "0", "0", "0", "1.0", "1.0", "(", "0", "0", "128", ")", "(", "0", "1", "128", ")", "(", "1", "0", "128", ")", "GROUND1_6", "0", "0", "0", "1.0", "1.0", "}", "}", "{", "\"classname\"", "\"info_player_start\"", "\"origin\"", "\"256 384 160\"", "}"};

	yyscan_t scanner;
	yylex_init(&scanner);

	int succeed = 0;

	FILE *tmpFile = tmpfile();
	assert(tmpFile != NULL);
	Error err = fprintf(tmpFile, "%s", source);
	assert(err != -1);
	err = fseek(tmpFile, 0, SEEK_SET);
	assert(err == 0);

	yyset_in(tmpFile, scanner);

	YYSTYPE value;

	yytoken_kind_t tok = yylex(&value, scanner);

	size_t i = 0;
	const size_t tokCnt = sizeof(expectedTokens) / sizeof(yytoken_kind_t);

	for (;i < tokCnt && tok != YYEOF; i++, tok = yylex(&value, scanner)) {
		if (tok == YYerror || tok == YYUNDEF) {
			PRINT_ERR("token %lu: unknown token %s\n", i,
				  yyget_text(scanner));
			succeed = -1;
		}
		if (expectedTokens[i] != tok) {
			PRINT_ERR("token %lu: expected token %d, got %d\n",
			      i, expectedTokens[i], tok);
			succeed = -1;
		}
		if (strcmp(expectedLexemes[i], yyget_text(scanner)) != 0) {
			PRINT_ERR("token %lu: expected lexeme %s, got %s\n",
			      i, expectedLexemes[i], yyget_text(scanner));
			succeed = -1;
		}
		if (tok == TOKEN_STRING) {
			free(value.string);
		} else if (tok == TOKEN_TEXTURE) {
			free(value.texture);			
		}
	}

	if (tok != YYEOF) {
		PRINT_ERR("didn't get EOF past end of input\n");
		succeed = -1;
	}

	if (i < tokCnt) {
		PRINT_ERR("Received EOF early at token %lu: %s\n", i,
			  yyget_text(scanner));
		succeed = -1;
	}

	yylex_destroy(scanner);
	fclose(tmpFile);

	return succeed;
}
