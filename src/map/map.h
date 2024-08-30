#pragma once

#include "common.h"
#include "dictionary.h"

typedef enum MapPoint {
	POINT_1_X = 0,
	POINT_1_Y,
	POINT_1_Z,
	POINT_2_X,
	POINT_2_Y,
	POINT_2_Z,
	POINT_3_X,
	POINT_3_Y,
	POINT_3_Z,
} MapPoint;

typedef enum MapAxis {
	AXIS_X = 0,
	AXIS_Y,
	AXIS_Z,
} MapAxis;

typedef struct MapFace {
	float points[9];
	struct MapTexture {
		char *name;
		float uAxis[3];
		float vAxis[3];
		float offsetX;
		float offsetY;
		float rotation;
		float scaleX;
		float scaleY;
	} texture;
} MapFace;

typedef struct MapBrush {
	MapFace *faces;
	size_t facesCount;
	size_t facesSize;
} MapBrush;

typedef struct MapEntity {
	Dictionary *attributes;
	MapBrush *brushes;
	size_t brushesCount;
	size_t brushesSize;
} MapEntity;

typedef struct Map {
	MapEntity *entities;
	size_t entitiesCount;
	size_t entitiesSize;
} Map;

/* Initialize a Brush structure, allocating memory for its faces. Return an
 * error code if initialization fails. */
Error MapBrushInit(MapBrush *out);

/* Initialize an Entity structure, allocating memory for its brushes and
 * attributes. Return an error code if initialization fails. */
Error MapEntityInit(MapEntity *out);

/* Initialize a Map structure, allocating memory for its entities
 * array. Return an error code if initialization fails. */
Error MapInit(Map *out);

/* Free the memory used by a Brush, including its faces. */
void MapBrushFree(MapBrush *out);

/* Free the memory used by an Entity, including its brushes and attributes. */
void MapEntityFree(MapEntity *out);

/* Free the memory used by a Map, including its entities. */
void MapFree(Map *out);

/* Create a deep copy of a Face structure. Returns an error code on memory
 * allocation failure. */
Error MapFaceDuplicate(MapFace *out, const MapFace *in);

/* Create a deep copy of a Brush structure. Returns an error code on memory
 * allocation failure. */
Error MapBrushDuplicate(MapBrush *out, const MapBrush *in);

/* Create a deep copy of an Entity structure. Returns an error code on memory
 * allocation failure. */
Error MapEntityDuplicate(MapEntity *out, const MapEntity *in);

/* Add a copy of a Face to an Brush's faces array. Returns an error code if
 * memory allocation fails. */
Error MapBrushAddFace(MapBrush *out, const MapFace *toCopy);

/* Add a copy of a Brush to an Entity's brushes array. Returns an error code if
 * memory allocation fails. */
Error MapEntityAddBrush(MapEntity *out, const MapBrush *toCopy);

/* Add a copy of an Entity to a Map's entities array. Returns an error code if
 * memory allocation fails. */
Error MapAddEntity(Map *out, const MapEntity *toCopy);

Error MapDuplicate(Map *out, const Map *in);

/* set *out to NULL in case the file is empty or there was a parsing error. The file position will be at the end if successful, and back where it originally was if not successful. Out will be null is in case of an error.*/
Error MapParse(Map **out, FILE *file);
