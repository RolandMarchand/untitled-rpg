%{
#include <stdio.h>
#include <stdlib.h>

#include "map.h"
#include "map_parser.h"
#include "map_scanner.h"

void yyerror(void *scanner, Map *map, const char *err);
%}

%define parse.trace
%define parse.error verbose

%define api.pure full
%lex-param {void *scanner}
%parse-param {Map *out} {void *scanner}

%union {
	float number;
	char* string;
	char* texture;
	Map map;
	MapEntity entity;
	MapFace face;
	MapBrush brush;
	Dictionary *attributes;
}


%token <number> TOKEN_NUMBER
%token <texture> TOKEN_TEXTURE
%token <string> TOKEN_STRING

%type <face> face
%type <brush> brush
%type <entity> brush_block
%type <attributes> attributes
%type <entity> entity
%type <map> entity_list
%type <map> file

%destructor { free($$); } <texture>
%destructor { free($$); } <string>
%destructor { MapFree(&$$); } <map>
%destructor { MapEntityFree(&$$); } <entity>
%destructor { free($$.texture.name); } <face>
%destructor { MapBrushFree(&$$); } <brush>
%destructor { DictionaryFree($$); } <attributes>

%%

file: entity_list YYEOF {
	MapDuplicate(out, &$1);
}
;

entity_list:
  entity {
	Map map;
	Error err = MapInit(&map); // TODO: manage error
	err = MapAddEntity(&map, &$1); // TODO: manage error
	MapEntityFree(&$1);
	$$ = map;
}
| entity_list entity {
	Error err = MapAddEntity(&$1, &$2); // TODO: manage error
	MapEntityFree(&$2);
	$$ = $1;
}
;

entity:
'{' attributes brush_block '}' {
	DictionaryFree($3.attributes);
	$3.attributes = $2;
	$$ = $3;
}
| '{' attributes '}' {
	MapEntity entity;
	Error err = MapEntityInit(&entity); // TODO: manage error
	DictionaryFree(entity.attributes);
	entity.attributes = $2;
	$$ = entity;
}
| '{' brush_block  '}' {
	$$ = $2;
}
| '{' '}' {
	MapEntity entity;
	Error err = MapEntityInit(&entity); // TODO: manage error
	$$ = entity;
}
;

attributes:
  TOKEN_STRING TOKEN_STRING {
	Dictionary *dict = DictionaryInit(0); // TODO: manage error
	DictionarySet(dict, $1, $2);
	free($1);
	free($2);
	$$ = dict;
}
| attributes TOKEN_STRING TOKEN_STRING {
	Error err = DictionarySet($1, $2, $3); // TODO: manage error
	free($2);
	free($3);
	$$ = $1;
}
;

brush_block:
 '{' brush '}' {
	MapEntity entity;
	Error err = MapEntityInit(&entity); // TODO: manage error
	err = MapEntityAddBrush(&entity, &$2); // TODO: manage error
	MapBrushFree(&$2);
	$$ = entity;
}
| brush_block '{' brush '}' {
	Error err = MapEntityAddBrush(&$1, &$3); // TODO: manage error
	MapBrushFree(&$3);
	$$ = $1;
}
;

brush:
  %empty {
	MapBrush brush;
	Error err = MapBrushInit(&brush); // TODO: manage error
	$$ = brush;
}
| brush face {
	Error err = MapBrushAddFace(&$1, &$2); // TODO: manage error
	free($2.texture.name);
	$$ = $1;
}
;

face:
  '(' TOKEN_NUMBER[x1] TOKEN_NUMBER[y1] TOKEN_NUMBER[z1] ')'
  '(' TOKEN_NUMBER[x2] TOKEN_NUMBER[y2] TOKEN_NUMBER[z2] ')'
  '(' TOKEN_NUMBER[x3] TOKEN_NUMBER[y3] TOKEN_NUMBER[z3] ')'
  TOKEN_TEXTURE[tex]
  '[' TOKEN_NUMBER[ux] TOKEN_NUMBER[uy] TOKEN_NUMBER[uz] TOKEN_NUMBER[offsetx] ']'
  '[' TOKEN_NUMBER[vx] TOKEN_NUMBER[vy] TOKEN_NUMBER[vz] TOKEN_NUMBER[offsety] ']'
  TOKEN_NUMBER[rot] TOKEN_NUMBER[scalex] TOKEN_NUMBER[scaley]
{
	MapFace face = (MapFace){
		.points = {$x1, $y1, $z1, $x2, $y2, $z2, $x3, $y3, $z3},
		.texture = {
			.name = $tex,
			.uAxis = {$ux, $uy, $uz},
			.vAxis = {$vx, $vy, $vz},
			.offsetX = $offsetx,
			.offsetY = $offsety,
			.rotation = $rot,
			.scaleX = $scalex,
			.scaleY = $scaley,
		}
	};
	$$ = face;
}

  | '(' TOKEN_NUMBER[x1] TOKEN_NUMBER[y1] TOKEN_NUMBER[z1] ')'
  '(' TOKEN_NUMBER[x2] TOKEN_NUMBER[y2] TOKEN_NUMBER[z2] ')'
  '(' TOKEN_NUMBER[x3] TOKEN_NUMBER[y3] TOKEN_NUMBER[z3] ')'
  TOKEN_TEXTURE[tex] TOKEN_NUMBER[offsetx] TOKEN_NUMBER[offsety] 
  TOKEN_NUMBER[rot] TOKEN_NUMBER[scalex] TOKEN_NUMBER[scaley]
  {
	MapFace face = (MapFace){
		.points = {$x1, $y1, $z1, $x2, $y2, $z2, $x3, $y3, $z3},
		.texture = {
			.name = $tex,
			.uAxis = {1.0f, 0.0f, 0.0f},
			.vAxis = {0.0f, -1.0f, 0.0f},
			.offsetX = $offsetx,
			.offsetY = $offsety,
			.rotation = $rot,
			.scaleX = $scalex,
			.scaleY = $scaley,
		}
	};
	$$ = face;
  }
;

%%

void yyerror(void *scanner, Map *map, const char *err)
{
	fprintf(stderr, "Parsing error: %s\n", err);
}
