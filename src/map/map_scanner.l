%{

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "common.h"
#include "map_parser.h"

void yyerror(const char *s);

%}

%option noyywrap yylineno

/* Definitions */
DIGIT   [0-9]
LETTER  [a-zA-Z_]
NUMBER   -?{DIGIT}+("."{DIGIT}+)?
TEXTURE {LETTER}({LETTER}|{DIGIT})*
STRING \"([^\\\"]|\\.)*\"

%%

"//".*                ; // Ignore comments (single-line)
{NUMBER}              { yylval.number = atof(yytext); return TOKEN_NUMBER; }
{TEXTURE}             { yylval.texture = strdup(yytext); return TOKEN_TEXTURE; }
{STRING}              { yylval.string = strdup(yytext); return TOKEN_STRING; }
"("                   { return '('; }
")"                   { return ')'; }
"{"                   { return '{'; }
"}"                   { return '}'; }
[ \t\n\r\v]+            ; // Ignore whitespace
<<EOF>>	              { yyterminate(); }
.                     { yyerror("Unknown token"); }

%%
