%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "map.h"
#include "map_parser.h"

void yyerror(const char *s);
%}

%option noyywrap yylineno nounistd never-interactive reentrant bison-bridge

/* Definitions */
DIGIT   [0-9]
LETTER  [a-zA-Z_]
NUMBER   -?{DIGIT}+("."{DIGIT}+)?
TEXTURE {LETTER}({LETTER}|{DIGIT})*
STRING \"([^\\\"]|\\.)*\"

%%

"//".*                ; // Ignore comments (single-line)
{NUMBER}              { yylval_param->number = atof(yytext); return TOKEN_NUMBER; }
{TEXTURE}             { yylval_param->texture = strdup(yytext); return TOKEN_TEXTURE; }
{STRING}              { yylval_param->string = strdup(yytext); return TOKEN_STRING; }
"("                   { return '('; }
")"                   { return ')'; }
"{"                   { return '{'; }
"}"                   { return '}'; }
"["                   { return '['; }
"]"                   { return ']'; }
[ \t\n\r\v]+            ; // Ignore whitespace
<<EOF>>	              { yyterminate(); }
.                     { yyerror("Unknown token"); }

%%
