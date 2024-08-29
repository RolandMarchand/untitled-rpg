#include "map.h"
#include "map_parser.h"
#include "map_scanner.h"

#define FACE_BUFFER_INIT_SIZE (8 * sizeof(Face))
#define BRUSH_BUFFER_INIT_SIZE (8 * sizeof(Brush))
#define ENTITY_BUFFER_INIT_SIZE (8 * sizeof(Entity))

Error BrushInit(Brush *out)
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

Error EntityInit(Entity *out)
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

void BrushFree(Brush *out)
{
	if (out == NULL) {
		return;
	}

	for (size_t i = 0; i < out->facesCount; i++) {
		free(out->faces[i].texture.name);
	}

	free(out->faces);
}

void EntityFree(Entity *out)
{
	if (out == NULL) {
		return;
	}

	for (size_t i = 0; i < out->brushesCount; i++) {
		BrushFree(&out->brushes[i]);
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
		EntityFree(&out->entities[i]);
	}

	free(out->entities);
}

Error FaceDuplicate(Face *out, const Face *in)
{
	if (out == NULL || in == NULL) {
		return ERR_NULL_REFERENCE;
	}

	memcpy(out, in, sizeof(Face));
	out->texture.name = strdup(in->texture.name);

	return out->texture.name == NULL
		? ERR_OUT_OF_MEMORY
		: ERR_OK;
}

Error BrushDuplicate(Brush *out, const Brush *in)
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
		Error err = FaceDuplicate(&out->faces[i], &in->faces[i]);
		if (err != ERR_OK) {
			return err;
		}
	}

	return ERR_OK;
}

Error EntityDuplicate(Entity *out, const Entity *in)
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
		Error err = BrushDuplicate(&out->brushes[i], &in->brushes[i]);
		if (err != ERR_OK) {
			return err;
		}
	}

	return ERR_OK;
}

Error BrushAddFace(Brush *out, const Face *toCopy)
{
	/* Increase the size of the array if needed. */
	if (out->facesCount * sizeof(Face) == out->facesSize) {
		out->facesSize *= 2;
		out->faces = realloc(out->faces, out->facesSize);
		if (out->faces == NULL) {
			return ERR_OUT_OF_MEMORY;
		}
	}

	Error err = FaceDuplicate(&out->faces[out->facesCount], toCopy);
	out->facesCount++;

	return err;
}

Error EntityAddBrush(Entity *out, const Brush *toCopy)
{
	/* Increase the size of the array if needed. */
	if (out->brushesCount * sizeof(Face) == out->brushesSize) {
		out->brushesSize *= 2;
		out->brushes = realloc(out->brushes, out->brushesSize);
		if (out->brushes == NULL) {
			return ERR_OUT_OF_MEMORY;
		}
	}

	Error err = BrushDuplicate(&out->brushes[out->brushesCount], toCopy);
	out->brushesCount++;

	return err;
}

Error MapAddEntity(Map *out, const Entity *toCopy)
{
	/* Increase the size of the array if needed. */
	if (out->entitiesCount * sizeof(Entity) == out->entitiesSize) {
		out->entitiesSize *= 2;
		out->entities = realloc(out->entities, out->entitiesSize);
		if (out->entities == NULL) {
			return ERR_OUT_OF_MEMORY;
		}
	}

	Error err = EntityDuplicate(&out->entities[out->entitiesCount], toCopy);
	out->entitiesCount++;

	return err;
}

Error MapParse(Map **out, const char *in)
{
	yyin = fopen(in, "r");
	yyparse(out);

	return ERR_OK;
}
