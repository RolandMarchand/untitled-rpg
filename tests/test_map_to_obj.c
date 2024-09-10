#include "map/map.h"

int main()
{
	const char *test = "// Game: Generic\n"
"// Format: Standard\n"
"// entity 0\n"
"{\n"
"\"classname\" \"worldspawn\"\n"
"\"_tb_textures\" \"textures\"\n"
"// brush 0\n"
"{\n"
"( -32 32 32 ) ( -32 -32 32 ) ( -32 -32 -32 ) BOOKSHL2 0 0 0 1 1\n"
"( -32 -32 32 ) ( 32 -32 32 ) ( 32 -32 -32 ) BOOKSHL2 0 0 0 1 1\n"
"( 32 -32 -32 ) ( 32 32 -32 ) ( -32 32 -32 ) BOOKSHL2 0 0 0 1 1\n"
"( -32 32 32 ) ( 32 32 32 ) ( 32 -32 32 ) BOOKSHL2 0 0 0 1 1\n"
"( 32 32 -32 ) ( 32 32 32 ) ( -32 32 32 ) BOOKSHL2 0 0 0 1 1\n"
"( 32 -32 32 ) ( 32 32 32 ) ( 32 32 -32 ) BOOKSHL2 0 0 0 1 1\n"
"}\n"
"}\n"
"// entity 1\n"
"{\n"
"\"classname\" \"func_group\"\n"
"\"_tb_type\" \"_tb_layer\"\n"
"\"_tb_name\" \"Layer2\"\n"
"\"_tb_id\" \"2\"\n"
"\"_tb_layer_sort_index\" \"0\"\n"
"}\n";
	FILE *source = tmpfile();
	fprintf(source, "%s", test);
	fseek(source, 0, SEEK_SET);
	Map map;
	MapParse(&map, source);
	MapToObj(&map, NULL);
	fclose(source);
}
