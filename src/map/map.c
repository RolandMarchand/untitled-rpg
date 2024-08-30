#include "map.h"
#include "map_parser.h"
#include "map_scanner.h"

#define FACE_BUFFER_INIT_SIZE (8 * sizeof(MapFace))
#define BRUSH_BUFFER_INIT_SIZE (8 * sizeof(MapBrush))
#define ENTITY_BUFFER_INIT_SIZE (8 * sizeof(MapEntity))

Error MapBrushInit(MapBrush *out)
{
	if (out == NULL) {
		return ERR_NULL_REFERENCE;
	}
	out->facesSize = FACE_BUFFER_INIT_SIZE;
	out->faces = malloc(out->facesSize);
	out->facesCount = 0;
	return out->faces == NULL
		? ERR_OUT_OF_MEMORY
		: ERR_OK;
}

Error MapEntityInit(MapEntity *out)
{
	if (out == NULL) {
		return ERR_NULL_REFERENCE;
	}
	out->brushesSize = BRUSH_BUFFER_INIT_SIZE;
	out->brushes = malloc(out->brushesSize);
	out->brushesCount = 0;
	out->attributes = DictionaryInit(0);
	int oom = out->brushes == NULL || out->attributes == NULL;
	return oom
		? ERR_OUT_OF_MEMORY
		: ERR_OK;
}

Error MapInit(Map *out)
{
	if (out == NULL) {
		return ERR_NULL_REFERENCE;
	}
	out->entitiesSize = ENTITY_BUFFER_INIT_SIZE;
	out->entities = malloc(out->entitiesSize);
	out->entitiesCount = 0;
	return out->entities == NULL
		? ERR_OUT_OF_MEMORY
		: ERR_OK;
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

	return out->texture.name == NULL
		? ERR_OUT_OF_MEMORY
		: ERR_OK;
}

Error MapBrushDuplicate(MapBrush *out, const MapBrush *in)
{
	if (out == NULL || in == NULL) {
		return ERR_NULL_REFERENCE;
	}

	out->facesCount = in->facesCount;
	out->facesSize = in->facesSize;
	out->faces = malloc(out->facesSize);

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
	out->brushes = malloc(out->brushesSize);
	out->attributes = DictionaryDuplicate(in->attributes);

	if (out->brushes == NULL || out->attributes == NULL) {
		return ERR_OUT_OF_MEMORY;
	}

	for (size_t i = 0; i < out->brushesCount; i++) {
		Error err = MapBrushDuplicate(&out->brushes[i], &in->brushes[i]);
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
		out->faces = realloc(out->faces, out->facesSize);
		if (out->faces == NULL) {
			return ERR_OUT_OF_MEMORY;
		}
	}

	Error err = MapFaceDuplicate(&out->faces[out->facesCount], toCopy);
	out->facesCount++;

	return err;
}

Error MapEntityAddBrush(MapEntity *out, const MapBrush *toCopy)
{
	/* Increase the size of the array if needed. */
	if (out->brushesCount * sizeof(MapFace) == out->brushesSize) {
		out->brushesSize *= 2;
		out->brushes = realloc(out->brushes, out->brushesSize);
		if (out->brushes == NULL) {
			return ERR_OUT_OF_MEMORY;
		}
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
		out->entities = realloc(out->entities, out->entitiesSize);
		if (out->entities == NULL) {
			return ERR_OUT_OF_MEMORY;
		}
	}

	Error err = MapEntityDuplicate(&out->entities[out->entitiesCount], toCopy);
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
		Error err = MapEntityDuplicate(&out->entities[i], &in->entities[i]);
		if (err != ERR_OK) {
			return err;
		}
	}

	return ERR_OK;
}

Error MapParse(Map **out, FILE *file)
{
	if (out == NULL || file == NULL) {
		return ERR_NULL_REFERENCE;
	}

	yyin = file;
	if (yyin == NULL) {
		
		return ERR_FILE_NOT_FOUND;
	}

	yylineno = 1;

	long originalPosition = ftell(file);

	if (yyparse(out) != 0) {
		yyrestart(yyin);
		if (fseek(file, originalPosition, SEEK_SET) != 0) {
			return ERR_FILE_SEEK_FAILURE;
		}
		return ERR_SYNTAX_ERROR;
	}

	yyrestart(yyin);
	return ERR_OK;
}
