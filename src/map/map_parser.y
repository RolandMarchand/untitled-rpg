%{
#include <stdio.h>
#include <stdlib.h>

#include "map_scanner.h"

void yyerror(const char *s);
%}

%define parse.error verbose

%union {
	float number;
	char* string;
	char* texture;
}

%token <number> TOKEN_NUMBER
%token <texture> TOKEN_TEXTURE
%token <string> TOKEN_STRING

%%
file:
  %empty
| entity_block_list
;

entity_block_list:
  entity_block
| entity_block_list entity_block
;

entity_block:
'{' entity_attributes brush_block '}' { printf("=========\n"); }
| '{' entity_attributes '}' { printf("=========\n"); }
| '{' brush_block  '}' { printf("=========\n"); }
| '{' '}' { printf("=========\n"); }
;

entity_attributes: entity { printf("---------\n"); }
;

entity:
  key_value
| entity key_value
;

key_value: TOKEN_STRING TOKEN_STRING { printf("attrib: %s %s\n", $1, $2); }
;

brush_block:
 '{' brush '}' { printf("---------\n"); }
| brush_block '{' brush '}' 
;

brush:
  %empty
| brush face
;

face:
  '(' TOKEN_NUMBER[p1x] TOKEN_NUMBER[p1y] TOKEN_NUMBER[p1z] ')'
  '(' TOKEN_NUMBER[p2x] TOKEN_NUMBER[p2y] TOKEN_NUMBER[p2z] ')'
  '(' TOKEN_NUMBER[p3x] TOKEN_NUMBER[p3y] TOKEN_NUMBER[p3z] ')'
  TOKEN_TEXTURE[tex]
  TOKEN_NUMBER[xoff] TOKEN_NUMBER[yoff] TOKEN_NUMBER[rot] TOKEN_NUMBER[xscale] TOKEN_NUMBER[yscale]
  {
	  printf("p1(x: %g, y: %g, z: %g), p2(x: %g, y: %g, z: %g), p3(x: %g, y: %g, z: %g), texture: %s, x_off: %g, y_off: %g, rot: %g, x_scale: %g, y_scale: %g\n",
		 $p1x, $p1y, $p1z, $p2x, $p2y, $p2z, $p3x, $p3y, $p3z, $tex, $xoff, $yoff, $rot, $xscale, $yscale);
  }
;

%%

void yyerror(const char *s) {
	fprintf(stderr, "Error: %d: %s: %s\n", yylineno, s, yytext);
}
