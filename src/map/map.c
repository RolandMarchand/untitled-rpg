#include "common.h"
#include "dictionary.h"
#include "map.h"
#include "map_parser.h"
#include "map_scanner.h"
#include "obj.h"
#include "vector3.h"

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
	FACE_BUFFER_INIT_SIZE = (8 * sizeof(MapFace)),
	BRUSH_BUFFER_INIT_SIZE = (8 * sizeof(MapBrush)),
	ENTITY_BUFFER_INIT_SIZE = (8 * sizeof(MapEntity)),
};

Error MapBrushInit(MapBrush *out)
{
	if (out == NULL) {
		return ERR_NULL_REFERENCE;
	}
	out->facesSize = FACE_BUFFER_INIT_SIZE;
	out->faces = malloc(out->facesSize);
	out->facesCount = 0;
	return out->faces == NULL ? ERR_OUT_OF_MEMORY : ERR_OK;
}

Error MapEntityInit(MapEntity *out)
{
	if (out == NULL) {
		return ERR_NULL_REFERENCE;
	}
	out->brushesSize = BRUSH_BUFFER_INIT_SIZE;
	out->brushes = malloc(out->brushesSize);
	if (out->brushes == NULL) {
		return ERR_OUT_OF_MEMORY;
	}
	out->brushesCount = 0;
	out->attributes = DictionaryInit(0);
	if (out->attributes == NULL) {
		free(out->brushes);
		return ERR_OUT_OF_MEMORY;
	}
	return ERR_OK;
}

Error MapInit(Map *out)
{
	if (out == NULL) {
		return ERR_NULL_REFERENCE;
	}
	out->entitiesSize = ENTITY_BUFFER_INIT_SIZE;
	out->entities = malloc(out->entitiesSize);
	out->entitiesCount = 0;
	return out->entities == NULL ? ERR_OUT_OF_MEMORY : ERR_OK;
}

void MapBrushFree(MapBrush *out)
{
	if (out == NULL) {
		return;
	}

	for (size_t i = 0; i < out->facesCount; i++) {
		free(out->faces[i].texture.name);
	}

	free(out->faces);
}

void MapEntityFree(MapEntity *out)
{
	if (out == NULL) {
		return;
	}

	for (size_t i = 0; i < out->brushesCount; i++) {
		MapBrushFree(&out->brushes[i]);
	}

	free(out->brushes);
	DictionaryFree(out->attributes);
}

void MapFree(Map *out)
{
	if (out == NULL) {
		return;
	}

	for (size_t i = 0; i < out->entitiesCount; i++) {
		MapEntityFree(&out->entities[i]);
	}

	free(out->entities);
}

Error MapFaceDuplicate(MapFace *out, const MapFace *in)
{
	if (out == NULL || in == NULL) {
		return ERR_NULL_REFERENCE;
	}

	memcpy(out, in, sizeof(MapFace));
	out->texture.name = strdup(in->texture.name);

	return out->texture.name == NULL ? ERR_OUT_OF_MEMORY : ERR_OK;
}

Error MapBrushDuplicate(MapBrush *out, const MapBrush *in)
{
	if (out == NULL || in == NULL) {
		return ERR_NULL_REFERENCE;
	}

	out->facesCount = in->facesCount;
	out->facesSize = in->facesSize;
	out->faces = malloc(in->facesSize);

	if (out->faces == NULL) {
		return ERR_OUT_OF_MEMORY;
	}

	for (size_t i = 0; i < out->facesCount; i++) {
		Error err = MapFaceDuplicate(&out->faces[i], &in->faces[i]);
		if (err != ERR_OK) {
			return err;
		}
	}

	return ERR_OK;
}

Error MapEntityDuplicate(MapEntity *out, const MapEntity *in)
{
	if (out == NULL || in == NULL) {
		return ERR_NULL_REFERENCE;
	}

	out->brushesCount = in->brushesCount;
	out->brushesSize = in->brushesSize;
	out->brushes = malloc(in->brushesSize);
	out->attributes = DictionaryDuplicate(in->attributes);

	if (out->brushes == NULL || out->attributes == NULL) {
		return ERR_OUT_OF_MEMORY;
	}

	for (size_t i = 0; i < out->brushesCount; i++) {
		Error err =
			MapBrushDuplicate(&out->brushes[i], &in->brushes[i]);
		if (err != ERR_OK) {
			return err;
		}
	}

	return ERR_OK;
}

Error MapBrushAddFace(MapBrush *out, const MapFace *toCopy)
{
	/* Increase the size of the array if needed. */
	if (out->facesCount * sizeof(MapFace) == out->facesSize) {
		out->facesSize *= 2;
		MapFace *faces = realloc(out->faces, out->facesSize);
		if (faces == NULL) {
			return ERR_OUT_OF_MEMORY;
		}
		out->faces = faces;
	}

	Error err = MapFaceDuplicate(&out->faces[out->facesCount], toCopy);
	out->facesCount++;

	return err;
}

Error MapEntityAddBrush(MapEntity *out, const MapBrush *toCopy)
{
	/* Increase the size of the array if needed. */
	if (out->brushesCount * sizeof(MapBrush) == out->brushesSize) {
		out->brushesSize *= 2;
		MapBrush *brushes = realloc(out->brushes, out->brushesSize);
		if (brushes == NULL) {
			return ERR_OUT_OF_MEMORY;
		}
		out->brushes = brushes;
	}

	Error err = MapBrushDuplicate(&out->brushes[out->brushesCount], toCopy);
	out->brushesCount++;

	return err;
}

Error MapAddEntity(Map *out, const MapEntity *toCopy)
{
	/* Increase the size of the array if needed. */
	if (out->entitiesCount * sizeof(MapEntity) == out->entitiesSize) {
		out->entitiesSize *= 2;
		MapEntity *entities = realloc(out->entities, out->entitiesSize);
		if (entities == NULL) {
			return ERR_OUT_OF_MEMORY;
		}
		out->entities = entities;
	}

	Error err =
		MapEntityDuplicate(&out->entities[out->entitiesCount], toCopy);
	out->entitiesCount++;

	return err;
}

Error MapDuplicate(Map *out, const Map *in)
{
	if (out == NULL || in == NULL) {
		return ERR_NULL_REFERENCE;
	}

	out->entitiesCount = in->entitiesCount;
	out->entitiesSize = in->entitiesSize;
	out->entities = malloc(out->entitiesSize);

	if (out->entities == NULL) {
		return ERR_OUT_OF_MEMORY;
	}

	for (size_t i = 0; i < out->entitiesCount; i++) {
		Error err =
			MapEntityDuplicate(&out->entities[i], &in->entities[i]);
		if (err != ERR_OK) {
			return err;
		}
	}

	return ERR_OK;
}

Error MapParse(Map *out, FILE *file)
{
	if (out == NULL || file == NULL) {
		return ERR_NULL_REFERENCE;
	}

	yyscan_t scanner = NULL;
	yylex_init(&scanner);

	yyset_in(file, scanner);
	//	yyset_lineno(1, scanner);
	long originalPosition = ftell(file);

	if (yyparse(out, scanner) != 0) {
		if (fseek(file, originalPosition, SEEK_SET) != 0) {
			yylex_destroy(scanner);
			return ERR_FILE_SEEK_FAILURE;
		}
		yylex_destroy(scanner);
		return ERR_INVALID_SYNTAX;
	}

	yylex_destroy(scanner);
	return ERR_OK;
}

static size_t MapBrushCount(const Map *map)
{
	if (map == NULL) {
		return 0;
	}

	size_t ret = 0;

	for (size_t i = 0; i < map->entitiesCount; i++) {
		ret += map->entities[i].brushesCount;
	}

	return ret;
}

static ObjObject BrushToObjObject(const MapBrush *in)
{
	struct ALIGN(128){
		Plane plane;
		size_t verticesCount;
		Vector3 vertices[5];
	} f[32] = {0};

	Plane *faces = malloc(in->facesCount * sizeof(Plane));
	for (int i = 0; i < in->facesCount; i++) {
		faces[i] = MapFaceToPlane(&in->faces[i]);
		f[i].plane = faces[i];
		printf("Face %d: %g\t%g\t%g\n", i, faces[i].normal.x, -faces[i].normal.z, faces[i].normal.y);
	}

	for (int i = 0; i < in->facesCount - 2; i++) {
		for (int j = i; j < in->facesCount - 1; j++) {
			for (int k = j; k < in->facesCount; k++) {
				if (i == j || i == k || j == k) {
					continue;
				}

				bool legal = true;
				Vector3 newVertex;
				bool intersects = GetIntersection(faces[i], faces[j], faces[k],
						&newVertex);
				if (!intersects) {
					continue;
				}

				for (int m = 0; legal && m < in->facesCount; m++) {
					float distanceFromPlane = Dot(faces[m].normal, newVertex) - faces[m].d;
					bool pointInsidePlane = NearlyEqual(distanceFromPlane, 0, FLT_EPSILON)
						|| LessThan(distanceFromPlane, 0, FLT_EPSILON);
					legal = legal && pointInsidePlane;
				}

				if (legal) {
					f[i].vertices[f[i].verticesCount++] = newVertex;
					f[j].vertices[f[j].verticesCount++] = newVertex;
					f[k].vertices[f[k].verticesCount++] = newVertex;
				}
			}
		}
	}

	for (int i = 0; i < in->facesCount; i++) {
		printf("f [%g, %g, %g]: ", f[i].plane.normal.x, -f[i].plane.normal.z, f[i].plane.normal.y);
		for (int j = 0; j < f[i].verticesCount; j++) {
			printf("(%g, %g, %g) ", f[i].vertices[j].x, -f[i].vertices[j].z, f[i].vertices[j].y);
		}
		printf("\n");
	}

	free(faces);
	ObjObject o = {0};
	return o;
}

Error MapToObj(const Map *in, ObjFile *out)
{
	for (int i = 0; i < in->entitiesCount; i++) {
		for (int j = 0; j < in->entities[i].brushesCount; j++) {
			BrushToObjObject(&in->entities[i].brushes[j]);
		}
	}

	return ERR_OK;
}

