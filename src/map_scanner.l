%top{
	typedef enum {
		TOKEN_NUMBER,
		TOKEN_TEXTURE,
		TOKEN_STRING,
		TOKEN_OPEN_PAREN = '(',
		TOKEN_CLOSE_PAREN = ')',
		TOKEN_OPEN_CURLY = '{',
		TOKEN_CLOSE_CURLY = '}',
		TOKEN_ERROR,
		TOKEN_EOF,
	} Token;
}
%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "common.h"



/* Function declarations */
void yyerror(const char *s);
%}

%option noyywrap

/* Definitions */
DIGIT   [0-9]
LETTER  [a-zA-Z_]
NUMBER   -?{DIGIT}+("."{DIGIT}+)?
TEXTURE {LETTER}({LETTER}|{DIGIT})*
STRING \"([^\\\"]|\\.)*\"

%%

"//".*                ; // Ignore comments (single-line)
{NUMBER}              { return TOKEN_NUMBER; }
{TEXTURE}             { return TOKEN_TEXTURE; }
{STRING}              { return TOKEN_STRING; }
"("                   { return TOKEN_OPEN_PAREN; }
")"                   { return TOKEN_CLOSE_PAREN; }
"{"                   { return TOKEN_OPEN_CURLY; }
"}"                   { return TOKEN_CLOSE_CURLY; }
[ \t\n\r\v]+            ; // Ignore whitespace
.                     { return TOKEN_ERROR; }
<<EOF>>	              { return TOKEN_EOF; }

%%

void yyerror(const char *s) {
	ERROR("%s\n", s);
}
