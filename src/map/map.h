#pragma once

#include "dictionary.h"

typedef enum Point {
	POINT_1_X = 0,
	POINT_1_Y,
	POINT_1_Z,
	POINT_2_X,
	POINT_2_Y,
	POINT_2_Z,
	POINT_3_X,
	POINT_3_Y,
	POINT_3_Z,
} Point;

typedef enum Axis {
	AXIS_X = 0,
	AXIS_Y,
	AXIS_Z,
} Axis;

typedef struct Face {
	float points[9];
	struct {
		char *name;
		float uAxis[3];
		float vAxis[3];
		float offsetX;
		float offsetY;
		float rotation;
		float scaleX;
		float scaleY;
	} texture;
} Face;

typedef struct Brush {
	Face *faces;
	size_t facesCount;
	size_t facesSize;	
} Brush;

typedef struct Entity {
	Dictionary *attributes;
	Brush *brushes;
	size_t brushesCount;
	size_t brushesSize;
} Entity;

typedef struct Map {
	Entity *entities;
	size_t entitiesCount;
	size_t entitiesSize;
} Map;

/* Initialize a Brush structure, allocating memory for its faces. Return an
 * error code if initialization fails. */
Error BrushInit(Brush *out);

/* Initialize an Entity structure, allocating memory for its brushes and
 * attributes. Return an error code if initialization fails. */
Error EntityInit(Entity *out);

/* Initialize a Map structure, allocating memory for its entities
 * array. Return an error code if initialization fails. */
Error MapInit(Map *out);

/* Free the memory used by a Brush, including its faces. */
void BrushFree(Brush *out);

/* Free the memory used by an Entity, including its brushes and attributes. */
void EntityFree(Entity *out);

/* Free the memory used by a Map, including its entities. */
void MapFree(Map *out);

/* Create a deep copy of a Face structure. Returns an error code on memory
 * allocation failure. */
Error FaceDuplicate(Face *out, const Face *in);

/* Create a deep copy of a Brush structure. Returns an error code on memory
 * allocation failure. */
Error BrushDuplicate(Brush *out, const Brush *in);

/* Create a deep copy of an Entity structure. Returns an error code on memory
 * allocation failure. */
Error EntityDuplicate(Entity *out, const Entity *in);

/* Add a copy of a Face to an Brush's faces array. Returns an error code if
 * memory allocation fails. */
Error BrushAddFace(Brush *out, const Face *toCopy);

/* Add a copy of a Brush to an Entity's brushes array. Returns an error code if
 * memory allocation fails. */
Error EntityAddBrush(Entity *out, const Brush *toCopy);

/* Add a copy of an Entity to a Map's entities array. Returns an error code if
 * memory allocation fails. */
Error MapAddEntity(Map *out, const Entity *toCopy);

Error MapParse(Map **out, const char *in);
