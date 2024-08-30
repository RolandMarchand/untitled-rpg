#include "map/map.h"
#include <assert.h>

#define ASSERT(test, ...)					\
	do {							\
		if (!(test)) {					\
			fprintf(stderr, "%d: ", __LINE__);	\
			PRINT_ERR(__VA_ARGS__);			\
			succeed = -1;				\
		}						\
	} while (0)

int succeed = 0;
/* Quake Notation. */
const char *validSource1 = "{\n"
	" \"sounds\" \"1\"\n"
	" \"classname\" \"worldspawn\"\n"
	" \"wad\" \"/gfx/base.wad\"\n"
	" \"worldtype\" \"0\"\n"
	" {\n"
	"  ( -128 0 0 ) ( 128.2 -1.0 0 ) ( 128 0 1 ) GROUND1_6 0 0 0 1.0 1.0\n"
	"  ( 256 0 0 ) ( 256 0 1 ) ( 256 1 0 ) GROUND1_6 0 0 0 1.0 1.0\n"
	"  ( 0 128 0 ) ( 0 128 1 ) ( 1 128 0 ) GROUND1_6 0 0 0 1.0 1.0\n"
	"  ( 0 384 0 ) ( 1 384 0 ) ( 0 384 1 ) GROUND1_6 0 0 0 1.0 1.0\n"
	"  ( 0 0 64 ) ( 1 0 64 ) ( 0 1 64 ) GROUND1_6 0 0 0 1.0 1.0\n"
	"  ( 0 0 128 ) ( 0 1 128 ) ( 1 0 128 ) GROUND1_6 0 0 0 1.0 1.0\n"
	" }\n"
	" {\n"
	"  ( -128 0 0 ) ( 128.2 -1.0 0 ) ( 128 0 1 ) GROUND1_6 0 0 0 1.0 1.0\n"
	"  ( 256 0 0 ) ( 256 0 1 ) ( 256 1 0 ) GROUND1_6 0 0 0 1.0 1.0\n"
	"  ( 0 128 0 ) ( 0 128 1 ) ( 1 128 0 ) GROUND1_6 0 0 0 1.0 1.0\n"
	"  ( 0 384 0 ) ( 1 384 0 ) ( 0 384 1 ) GROUND1_6 0 0 0 1.0 1.0\n"
	"  ( 0 0 64 ) ( 1 0 64 ) ( 0 1 64 ) GROUND1_6 0 0 0 1.0 1.0\n"
	"  ( 0 0 128 ) ( 0 1 128 ) ( 1 0 128 ) GROUND1_6 0 0 0 1.0 1.0\n"
	" }\n"
	"}\n"
	"{\n"
	" \"classname\" \"info_player_start\"\n"
	" \"origin\" \"256 384 160\"\n"
	"}\n";
/* Half-Life notation. */
const char *validSource2 = "{\n"
	" \"sounds\" \"1\"\n"
	" \"classname\" \"worldspawn\"\n"
	" \"wad\" \"/gfx/base.wad\"\n"
	" \"worldtype\" \"0\"\n"
	" {\n"
	"  ( -128 0 0 ) ( 128.2 -1.0 0 ) ( 128 0 1 ) GROUND1_6 [ 1 0 0 0 ] [ 0 -1 0 0 ] 0 1.0 1.0\n"
	"  ( 256 0 0 ) ( 256 0 1 ) ( 256 1 0 ) GROUND1_6 [ 1 0 0 0 ] [ 0 -1 0 0 ] 0 1.0 1.0\n"
	"  ( 0 128 0 ) ( 0 128 1 ) ( 1 128 0 ) GROUND1_6 [ 1 0 0 0 ] [ 0 -1 0 0 ] 0 1.0 1.0\n"
	"  ( 0 384 0 ) ( 1 384 0 ) ( 0 384 1 ) GROUND1_6 [ 1 0 0 0 ] [ 0 -1 0 0 ] 0 1.0 1.0\n"
	"  ( 0 0 64 ) ( 1 0 64 ) ( 0 1 64 ) GROUND1_6 [ 1 0 0 0 ] [ 0 -1 0 0 ] 0 1.0 1.0\n"
	"  ( 0 0 128 ) ( 0 1 128 ) ( 1 0 128 ) GROUND1_6 [ 1 0 0 0 ] [ 0 -1 0 0 ] 0 1.0 1.0\n"
	" }\n"
	" {\n"
	"  ( -128 0 0 ) ( 128.2 -1.0 0 ) ( 128 0 1 ) GROUND1_6 [ 1 0 0 0 ] [ 0 -1 0 0 ] 0 1.0 1.0\n"
	"  ( 256 0 0 ) ( 256 0 1 ) ( 256 1 0 ) GROUND1_6 [ 1 0 0 0 ] [ 0 -1 0 0 ] 0 1.0 1.0\n"
	"  ( 0 128 0 ) ( 0 128 1 ) ( 1 128 0 ) GROUND1_6 [ 1 0 0 0 ] [ 0 -1 0 0 ] 0 1.0 1.0\n"
	"  ( 0 384 0 ) ( 1 384 0 ) ( 0 384 1 ) GROUND1_6 [ 1 0 0 0 ] [ 0 -1 0 0 ] 0 1.0 1.0\n"
	"  ( 0 0 64 ) ( 1 0 64 ) ( 0 1 64 ) GROUND1_6 [ 1 0 0 0 ] [ 0 -1 0 0 ] 0 1.0 1.0\n"
	"  ( 0 0 128 ) ( 0 1 128 ) ( 1 0 128 ) GROUND1_6 [ 1 0 0 0 ] [ 0 -1 0 0 ] 0 1.0 1.0\n"
	" }\n"
	"}\n"
	"{\n"
	" \"classname\" \"info_player_start\"\n"
	" \"origin\" \"256 384 160\"\n"
	"}\n";

int MapCompare(Map *a, Map *b)
{
	assert(a != NULL);
	assert(b != NULL);

	if (a == b) {
		return 0;
	}

	assert(a->entities != NULL);
	assert(a->entities != NULL);
	if (a->entities == b->entities) {
		return 0;
	}
	if (a->entitiesCount < b-> entitiesCount) {
		return -1;
	}
	if (a->entitiesCount > b-> entitiesCount) {
		return 1;
	}

	MapEntity *aEnt;
	MapEntity *bEnt;
	for (size_t i = 0; i < a->entitiesCount; i++) {
		aEnt = &a->entities[i];
		bEnt = &b->entities[i];
		assert(aEnt != NULL);
		assert(bEnt != NULL);

		if (aEnt == bEnt) {
			continue;
		}
		if (!DictionaryCompare(aEnt->attributes, aEnt->attributes)) {
			return aEnt->attributes->count < bEnt->attributes->count
				? -1
				: 1;
		}
		if (aEnt->brushes == bEnt->brushes) {
			continue;
		}
		if (aEnt->brushesCount < bEnt->brushesCount) {
			return -1;
		}
		if (aEnt->brushesCount > bEnt->brushesCount) {
			return 1;
		}

		MapBrush *aBr;
		MapBrush *bBr;
		for (size_t j = 0; j < aEnt->brushesCount; j++) {
			aBr = &aEnt->brushes[j];
			bBr = &bEnt->brushes[j];
			assert(aBr != NULL);
			assert(bBr != NULL);

			if (aBr == bBr) {
				continue;
			}
			if (aBr->faces == bBr->faces) {
				continue;
			}
			if (aBr->facesCount < bBr->facesCount) {
				return -1;
			}
			if (aBr->facesCount > bBr->facesCount) {
				return 1;
			}

			MapFace *aFa;
			MapFace *bFa;
			struct MapTexture *aTex;
			struct MapTexture *bTex;
			int cmp;
			for (size_t k = 0; k < aBr->facesCount; k++) {
				aFa = &aBr->faces[k];
				bFa = &bBr->faces[k];
				aTex = &aFa->texture;
				bTex = &bFa->texture;
				assert(aFa != NULL);
				assert(bFa != NULL);
				assert(aTex != NULL);
				assert(bTex != NULL);

				if (aFa == bFa) {
					continue;
				}

				cmp = memcmp(aFa->points, bFa->points,
					     sizeof(aFa->points));
				if (cmp != 0) {
					return cmp;
				}

				cmp = strcmp(aTex->name,
					     bTex->name);
				if (cmp != 0) {
					return cmp;
				}

				cmp = memcmp(aTex->uAxis,
					     bTex->uAxis,
					     sizeof(aTex->uAxis));
				if (cmp != 0) {
					return cmp;
				}

				cmp = memcmp(aTex->vAxis,
					     bTex->vAxis,
					     sizeof(aTex->vAxis));
				if (cmp != 0) {
					return cmp;
				}

				if (aTex->offsetX < bTex->offsetX) {
					return -1;
				}

				if (aTex->offsetX > bTex->offsetX) {
					return 1;
				}

				if (aTex->offsetY < bTex->offsetY) {
					return -1;
				}

				if (aTex->offsetY > bTex->offsetY) {
					return 1;
				}

				if (aTex->rotation < bTex->rotation) {
					return -1;
				}

				if (aTex->rotation > bTex->rotation) {
					return 1;
				}

				if (aTex->scaleX < bTex->scaleX) {
					return -1;
				}

				if (aTex->scaleX > bTex->scaleX) {
					return 1;
				}

				if (aTex->scaleX < bTex->scaleX) {
					return -1;
				}

				if (aTex->scaleX > bTex->scaleX) {
					return 1;
				}
			}
		}
	}

	return 0;
}

/* Initializes the provided parameters with a file stream, a Map object, and a
 * source string for testing purposes. Null arguments are ignored.If an error
 * occurs during initialization, an appropriate error code is returned. This
 * function expects that the arguments are freed with fclose() and MapFree()
 * respectively if ever provided. */
Error GetValidMap(FILE **fileOut, Map *mapOut)
{
	FILE *tmpFile = tmpfile();
	if (tmpFile == NULL) {
		return ERR_FILE_NOT_FOUND;
	}
	Error err = fprintf(tmpFile, "%s", validSource1);
	assert(err != -1);
	err = fseek(tmpFile, 0, SEEK_SET);
	assert(err == 0);
	
	Map tmpMap;
	err = MapParse(&tmpMap, tmpFile);
	assert(err == ERR_OK);
	err = fseek(tmpFile, 0, SEEK_SET);
	assert(err == 0);

	if (fileOut != NULL) {
		*fileOut = tmpFile;
	} else {
		fclose(tmpFile);
	}
	if (mapOut != NULL) {
		*mapOut = tmpMap;
	} else {
		MapFree(&tmpMap);
	}
	
	return ERR_OK;
}

/* Test the positive equality of maps. */
void TestMapCompare1()
{
	Map map1, map2;

	Error err = GetValidMap(NULL, &map1);
	assert(err == ERR_OK);
	err = GetValidMap(NULL, &map2);
	assert(err == ERR_OK);

	ASSERT(MapCompare(&map1, &map2) == 0,
	       "Expected both maps to be equal.\n");

	MapFree(&map1);
	MapFree(&map2);
}

/* Test the negative equality of maps. */
void TestMapCompare2()
{
	Map map1, map2;

	Error err = GetValidMap(NULL, &map1);
	assert(err == ERR_OK);
	err = GetValidMap(NULL, &map2);
	assert(err == ERR_OK);

	map1.entitiesCount--;

	ASSERT(MapCompare(&map1, &map2) != 0,
	       "Expected both maps to not be equal.\n");

	map1.entitiesCount++;

	MapFree(&map1);
	MapFree(&map2);
}

/* Test the negative equality of maps. */
void TestMapCompare3()
{
	Map map1, map2;

	Error err = GetValidMap(NULL, &map1);
	assert(err == ERR_OK);
	err = GetValidMap(NULL, &map2);
	assert(err == ERR_OK);

	map1.entities[0].brushesCount--;

	ASSERT(MapCompare(&map1, &map2) != 0,
	       "Expected both maps to not be equal.\n");

	map1.entities[0].brushesCount++;

	MapFree(&map1);
	MapFree(&map2);
}

/* Test the negative equality of maps. */
void TestMapCompare4()
{
	Map map1, map2;

	Error err = GetValidMap(NULL, &map1);
	assert(err == ERR_OK);
	err = GetValidMap(NULL, &map2);
	assert(err == ERR_OK);

	map1.entities[0].brushes[0].facesCount--;

	ASSERT(MapCompare(&map1, &map2) != 0,
	       "Expected both maps to not be equal.\n");

	map1.entities[0].brushes[0].facesCount++;

	MapFree(&map1);
	MapFree(&map2);
}

/* Test the negative equality of maps. */
void TestMapCompare5()
{
	Map map1, map2;

	Error err = GetValidMap(NULL, &map1);
	assert(err == ERR_OK);
	err = GetValidMap(NULL, &map2);
	assert(err == ERR_OK);

	map1.entities[0].brushes[0].faces[0].points[0] = -9000.0f;

	ASSERT(MapCompare(&map1, &map2) != 0,
	       "Expected both maps to not be equal\n");

	MapFree(&map1);
	MapFree(&map2);
}

/* Test the negative equality of maps. */
void TestMapCompare6()
{
	Map map1, map2;

	Error err = GetValidMap(NULL, &map1);
	assert(err == ERR_OK);
	err = GetValidMap(NULL, &map2);
	assert(err == ERR_OK);

	char *tmp = map1.entities[0].brushes[0].faces[0].texture.name;
	map1.entities[0].brushes[0].faces[0].texture.name = "potato";

	ASSERT(MapCompare(&map1, &map2) != 0,
	       "Expected both maps to not be equal\n");

	map1.entities[0].brushes[0].faces[0].texture.name = tmp;

	MapFree(&map1);
	MapFree(&map2);
}

/* Test the Quake notation. */
void TestSuccessfulParse1()
{
	MapFace face1 = {{-128, 0, 0, 128.2, -1, 0, 128, 0, 1},
			 {"GROUND1_6", {1, 0, 0}, {0, -1, 0}, 0, 0, 0, 1, 1}};
	MapFace face2 = {{256, 0, 0, 256, 0, 1, 256, 1, 0},
			 {"GROUND1_6", {1, 0, 0}, {0, -1, 0}, 0, 0, 0, 1, 1}};
	MapFace face3 = {{0, 128, 0, 0, 128, 1, 1, 128, 0},
			 {"GROUND1_6", {1, 0, 0}, {0, -1, 0}, 0, 0, 0, 1, 1}};
	MapFace face4 = {{0, 384, 0, 1, 384, 0, 0, 384, 1},
			 {"GROUND1_6", {1, 0, 0}, {0, -1, 0}, 0, 0, 0, 1, 1}};
	MapFace face5 = {{0, 0, 64, 1, 0, 64, 0, 1, 64},
			 {"GROUND1_6", {1, 0, 0}, {0, -1, 0}, 0, 0, 0, 1, 1}};
	MapFace face6 = {{0, 0, 128, 0, 1, 128, 1, 0, 128},
			 {"GROUND1_6", {1, 0, 0}, {0, -1, 0}, 0, 0, 0, 1, 1}};
	MapFace face7 = {{-128, 0, 0, 128.2, -1, 0, 128, 0, 1},
			 {"GROUND1_6", {1, 0, 0}, {0, -1, 0}, 0, 0, 0, 1, 1}};
	MapFace face8 = {{256, 0, 0, 256, 0, 1, 256, 1, 0},
			 {"GROUND1_6", {1, 0, 0}, {0, -1, 0}, 0, 0, 0, 1, 1}};
	MapFace face9 = {{0, 128, 0, 0, 128, 1, 1, 128, 0},
			 {"GROUND1_6", {1, 0, 0}, {0, -1, 0}, 0, 0, 0, 1, 1}};
	MapFace face10 = {{0, 384, 0, 1, 384, 0, 0, 384, 1},
			  {"GROUND1_6", {1, 0, 0}, {0, -1, 0}, 0, 0, 0, 1, 1}};
	MapFace face11 = {{0, 0, 64, 1, 0, 64, 0, 1, 64},
			  {"GROUND1_6", {1, 0, 0}, {0, -1, 0}, 0, 0, 0, 1, 1}};
	MapFace face12 = {{0, 0, 128, 0, 1, 128, 1, 0, 128},
			  {"GROUND1_6", {1, 0, 0}, {0, -1, 0}, 0, 0, 0, 1, 1}};

	Error err;
	Dictionary *attributes1 = DictionaryInit(0);
	err = DictionarySet(attributes1, "sounds", "1");
	assert(err == ERR_OK);
	err = DictionarySet(attributes1, "classname", "worldspawn");
	assert(err == ERR_OK);
	err = DictionarySet(attributes1, "wad", "/gfx/base.wad");
	assert(err == ERR_OK);
	err = DictionarySet(attributes1, "worldtype", "0");
	assert(err == ERR_OK);

	Dictionary *attributes2 = DictionaryInit(0);
	DictionarySet(attributes2, "classname", "info_player_start");
	assert(err == ERR_OK);
	DictionarySet(attributes2, "origin", "256 384 160");
	assert(err == ERR_OK);

	MapBrush brush1 = {(MapFace[]){face1, face2, face3, face4, face5, face6}, 6, 6 * sizeof(MapFace)};
	MapBrush brush2 = {(MapFace[]){face7, face8, face9, face10, face11, face12}, 6, 6 * sizeof(MapFace)};
	MapBrush brush3 = {(MapFace[]){}, 0, 0};

	MapEntity entity1 = {attributes1, (MapBrush[]){brush1, brush2}, 2, 2 * sizeof(MapBrush)};
	MapEntity entity2 = {attributes2, (MapBrush[]){}, 0, 0};

	Map map1 = (Map){(MapEntity[]){entity1, entity2}, 2, 2 * sizeof(MapEntity)};
	Map map2;

	FILE *source;
	err = GetValidMap(&source, NULL);
	assert(err == ERR_OK);
	err = MapParse(&map2, source);
	assert(err == ERR_OK);
	fclose(source);

	ASSERT(MapCompare(&map1, &map2) == 0,
	       "Expected both maps to be equal.\n");

	MapFree(&map2);
	DictionaryFree(attributes1);
	DictionaryFree(attributes2);
}

/* Test the Half-Life notation. */
void TestSuccessfulParse2()
{
	MapFace face1 = {{-128, 0, 0, 128.2, -1, 0, 128, 0, 1},
			 {"GROUND1_6", {1, 0, 0}, {0, -1, 0}, 0, 0, 0, 1, 1}};
	MapFace face2 = {{256, 0, 0, 256, 0, 1, 256, 1, 0},
			 {"GROUND1_6", {1, 0, 0}, {0, -1, 0}, 0, 0, 0, 1, 1}};
	MapFace face3 = {{0, 128, 0, 0, 128, 1, 1, 128, 0},
			 {"GROUND1_6", {1, 0, 0}, {0, -1, 0}, 0, 0, 0, 1, 1}};
	MapFace face4 = {{0, 384, 0, 1, 384, 0, 0, 384, 1},
			 {"GROUND1_6", {1, 0, 0}, {0, -1, 0}, 0, 0, 0, 1, 1}};
	MapFace face5 = {{0, 0, 64, 1, 0, 64, 0, 1, 64},
			 {"GROUND1_6", {1, 0, 0}, {0, -1, 0}, 0, 0, 0, 1, 1}};
	MapFace face6 = {{0, 0, 128, 0, 1, 128, 1, 0, 128},
			 {"GROUND1_6", {1, 0, 0}, {0, -1, 0}, 0, 0, 0, 1, 1}};
	MapFace face7 = {{-128, 0, 0, 128.2, -1, 0, 128, 0, 1},
			 {"GROUND1_6", {1, 0, 0}, {0, -1, 0}, 0, 0, 0, 1, 1}};
	MapFace face8 = {{256, 0, 0, 256, 0, 1, 256, 1, 0},
			 {"GROUND1_6", {1, 0, 0}, {0, -1, 0}, 0, 0, 0, 1, 1}};
	MapFace face9 = {{0, 128, 0, 0, 128, 1, 1, 128, 0},
			 {"GROUND1_6", {1, 0, 0}, {0, -1, 0}, 0, 0, 0, 1, 1}};
	MapFace face10 = {{0, 384, 0, 1, 384, 0, 0, 384, 1},
			  {"GROUND1_6", {1, 0, 0}, {0, -1, 0}, 0, 0, 0, 1, 1}};
	MapFace face11 = {{0, 0, 64, 1, 0, 64, 0, 1, 64},
			  {"GROUND1_6", {1, 0, 0}, {0, -1, 0}, 0, 0, 0, 1, 1}};
	MapFace face12 = {{0, 0, 128, 0, 1, 128, 1, 0, 128},
			  {"GROUND1_6", {1, 0, 0}, {0, -1, 0}, 0, 0, 0, 1, 1}};

	Error err;
	Dictionary *attributes1 = DictionaryInit(0);
	err = DictionarySet(attributes1, "sounds", "1");
	assert(err == ERR_OK);
	err = DictionarySet(attributes1, "classname", "worldspawn");
	assert(err == ERR_OK);
	err = DictionarySet(attributes1, "wad", "/gfx/base.wad");
	assert(err == ERR_OK);
	err = DictionarySet(attributes1, "worldtype", "0");
	assert(err == ERR_OK);

	Dictionary *attributes2 = DictionaryInit(0);
	DictionarySet(attributes2, "classname", "info_player_start");
	assert(err == ERR_OK);
	DictionarySet(attributes2, "origin", "256 384 160");
	assert(err == ERR_OK);

	MapBrush brush1 = {(MapFace[]){face1, face2, face3, face4, face5, face6}, 6, 6 * sizeof(MapFace)};
	MapBrush brush2 = {(MapFace[]){face7, face8, face9, face10, face11, face12}, 6, 6 * sizeof(MapFace)};
	MapBrush brush3 = {(MapFace[]){}, 0, 0};

	MapEntity entity1 = {attributes1, (MapBrush[]){brush1, brush2}, 2, 2 * sizeof(MapBrush)};
	MapEntity entity2 = {attributes2, (MapBrush[]){}, 0, 0};

	Map map1 = (Map){(MapEntity[]){entity1, entity2}, 2, 2 * sizeof(MapEntity)};
	Map map2;

	FILE *source = tmpfile();
	assert(source != NULL);
	err = fprintf(source, "%s", validSource2);
	assert(err != -1);
	err = fseek(source, 0, SEEK_SET);
	assert(err == 0);
	err = MapParse(&map2, source);
	assert(err == ERR_OK);
	fclose(source);

	ASSERT(MapCompare(&map1, &map2) == 0,
	       "Expected both maps to be equal.\n");

	MapFree(&map2);
	DictionaryFree(attributes1);
	DictionaryFree(attributes2);
}

/* Missing outer braces. */
void TestFailedParse1()
{
	const char *invalidSource = "\"classname\" \"worldspawn\"\n"
		"\"mapversion\" \"220\"\n"
		"\"wad\" \"/games/half-life/cstrike/cstrike.wad;/games/half-life/valve/halflife.wad\"\n"
		"{\n"
		"( 0 64 64 ) ( 64 64 64 ) ( 64 0 64 ) BCRATE02 [ 1 0 0 0 ] [ 0 -1 0 0 ] 0 1 1\n";

	FILE *source = tmpfile();
	assert(source != NULL);
	Error err = fprintf(source, "%s", invalidSource);
	assert(err != -1);
	err = fseek(source, 0, SEEK_SET);
	assert(err == 0);
	Map map;
	ASSERT(MapParse(&map, source) == ERR_SYNTAX_ERROR,
	       "Expected syntax to be invalid.\n");
	fclose(source);
}

/* Missing bracket. */
void TestFailedParse2()
{
	const char *invalidSource = "{\n"
		"\"classname\" \"worldspawn\"\n"
		"\"mapversion\" \"220\"\n"
		"\"wad\" \"/games/half-life/cstrike/cstrike.wad;/games/half-life/valve/halflife.wad\"\n"
		"{\n"
		"( 0 64 64 ) ( 64 64 64 ) ( 64 0 64 ) BCRATE02 [ 1 0 0 0 ] [ 0 -1 0 0 ] 0 1 1\n"
		"( 0 64 64 ) ( 64 64 64 ) ( 64 0 64 ) BCRATE02 [ 1 0 0 0 ] [ 0 -1 0 0 ] 0 1 1\n"
		"}\n";

	FILE *source = tmpfile();
	assert(source != NULL);
	Error err = fprintf(source, "%s", invalidSource);
	assert(err != -1);
	err = fseek(source, 0, SEEK_SET);
	assert(err == 0);
	Map map;
	ASSERT(MapParse(&map, source) == ERR_SYNTAX_ERROR,
	       "Expected syntax to be invalid.\n");
	fclose(source);
}

/* Extra-closing braces. */
void TestFailedParse3()
{
	const char *invalidSource = "{\n"
		"\"classname\" \"worldspawn\"\n"
		"\"mapversion\" \"220\"\n"
		"\"wad\" \"/games/half-life/cstrike/cstrike.wad;/games/half-life/valve/halflife.wad\"\n"
		"{\n"
		"( 0 64 64 ) ( 64 64 64 ) ( 64 0 64 ) BCRATE02 [ 1 0 0 0 ] [ 0 -1 0 0 ] 0 1 1\n"
		"}\n"
		"}\n"
		"}\n";

	FILE *source = tmpfile();
	assert(source != NULL);
	Error err = fprintf(source, "%s", invalidSource);
	assert(err != -1);
	err = fseek(source, 0, SEEK_SET);
	assert(err == 0);
	Map map;
	ASSERT(MapParse(&map, source) == ERR_SYNTAX_ERROR,
	       "Expected syntax to be invalid.\n");
	fclose(source);
}

/* Non-numeric values for coordinates. */
void TestFailedParse4()
{
	const char *invalidSource = "{\n"
		"\"classname\" \"worldspawn\"\n"
		"\"mapversion\" \"220\"\n"
		"\"wad\" \"/games/half-life/cstrike/cstrike.wad;/games/half-life/valve/halflife.wad\"\n"
		"{\n"
		"( a 64 64 ) ( 64 64 64 ) ( 64 0 64 ) BCRATE02 [ 1 0 0 0 ] [ 0 -1 0 0 ] 0 1 1\n"
		"}\n"
		"}\n";

	FILE *source = tmpfile();
	assert(source != NULL);
	Error err = fprintf(source, "%s", invalidSource);
	assert(err != -1);
	err = fseek(source, 0, SEEK_SET);
	assert(err == 0);
	Map map;
	ASSERT(MapParse(&map, source) == ERR_SYNTAX_ERROR,
	       "Expected syntax to be invalid.\n");
	fclose(source);
}

/* Incorrect vector length. */
void TestFailedParse5()
{
	const char *invalidSource = "{\n"
		"\"classname\" \"worldspawn\"\n"
		"\"mapversion\" \"220\"\n"
		"\"wad\" \"/games/half-life/cstrike/cstrike.wad;/games/half-life/valve/halflife.wad\"\n"
		"{\n"
		"( 0 64 ) ( 64 64 64 ) ( 64 0 64 ) BCRATE02 [ 1 0 0 0 ] [ 0 -1 0 0 ] 0 1 1\n"
		"}\n"
		"}\n";

	FILE *source = tmpfile();
	assert(source != NULL);
	Error err = fprintf(source, "%s", invalidSource);
	assert(err != -1);
	err = fseek(source, 0, SEEK_SET);
	assert(err == 0);
	Map map;
	ASSERT(MapParse(&map, source) == ERR_SYNTAX_ERROR,
	       "Expected syntax to be invalid.\n");
	fclose(source);
}

/* Extra field. */
void TestFailedParse6()
{
	const char *invalidSource = "{\n"
		"\"classname\" \"worldspawn\"\n"
		"\"mapversion\" \"220\"\n"
		"\"wad\" \"/games/half-life/cstrike/cstrike.wad;/games/half-life/valve/halflife.wad\"\n"
		"{\n"
		"( 0 64 64 ) ( 64 64 64 ) ( 64 0 64 ) BCRATE02 [ 1 0 0 0 ] [ 0 -1 0 0 ] 0 1 1\n"
		"extra_field\n"
		"}\n"
		"}\n";

	FILE *source = tmpfile();
	assert(source != NULL);
	Error err = fprintf(source, "%s", invalidSource);
	assert(err != -1);
	err = fseek(source, 0, SEEK_SET);
	assert(err == 0);
	Map map;
	ASSERT(MapParse(&map, source) == ERR_SYNTAX_ERROR,
	       "Expected syntax to be invalid.\n");
	fclose(source);
}

/* Mismatched attributes. */
void TestFailedParse7()
{
	const char *invalidSource = "{\n"
		"\"classname\" \"worldspawn\"\n"
		"\"mapversion\" \n"
		"\"wad\" \"/games/half-life/cstrike/cstrike.wad;/games/half-life/valve/halflife.wad\"\n"
		"{\n"
		"( 0 64 64 ) ( 64 64 64 ) ( 64 0 64 ) BCRATE02 [ 1 0 0 0 ] [ 0 -1 0 0 ] 0 1 1\n"
		"( 0 0 0 ) ( 64 0 0 ) ( 64 64 0 ) BCRATE02 [ 1 0 0 0 ] [ 0 -1 0 0 ] 0 1 1\n"
		"( 0 64 64 ) ( 0 0 64 ) ( 0 0 0 ) BCRATE02 [ 0 1 0 0 ] [ 0 0 -1 0 ] 0 1 1\n"
		"( 64 64 0 ) ( 64 0 0 ) ( 64 0 64 ) BCRATE02 [ 0 1 0 0 ] [ 0 0 -1 0 ] 0 1 1\n"
		"( 64 64 64 ) ( 0 64 64 ) ( 0 64 0 ) BCRATE02 [ 1 0 0 0 ] [ 0 0 -1 0 ] 0 1 1\n"
		"( 64 0 0 ) ( 0 0 0 ) ( 0 0 64 ) BCRATE02 [ 1 0 0 0 ] [ 0 0 -1 0 ] 0 1 1\n"
		"}\n";

	FILE *source = tmpfile();
	assert(source != NULL);
	Error err = fprintf(source, "%s", invalidSource);
	assert(err != -1);
	err = fseek(source, 0, SEEK_SET);
	assert(err == 0);
	Map map;
	ASSERT(MapParse(&map, source) == ERR_SYNTAX_ERROR,
	       "Expected syntax to be invalid.\n");
	fclose(source);
}

int main()
{
	TestMapCompare1();
	TestMapCompare2();
	TestMapCompare3();
	TestMapCompare4();
	TestMapCompare5();
	TestMapCompare6();
	TestSuccessfulParse1();
	TestSuccessfulParse2();
	TestFailedParse1();
	TestFailedParse2();
	TestFailedParse3();
	TestFailedParse4();
	TestFailedParse5();
	TestFailedParse6();
	TestFailedParse7();
	return succeed;
}
