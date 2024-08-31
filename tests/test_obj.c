#include "map/obj.h"
#include <assert.h>

#define ASSERT(test, ...)					\
	do {							\
		if (!(test)) {					\
			fprintf(stderr, "%d: ", __LINE__);	\
			PRINT_ERR(__VA_ARGS__);			\
			succeed = -1;				\
		}						\
	} while (0)


enum {
	LARGE_ELEMENT_COUNT = 1000,
	SMALL_ELEMENT_COUNT = 10,
};

int succeed = 0;

void TestFree()
{
	ObjFile obj;

	obj.materialLibrary = strdup("SHINY_METAL");
	assert(obj.materialLibrary != NULL);

	/* Fill up vertices. */
	obj.verticesCount = LARGE_ELEMENT_COUNT;
	obj.verticesSize = LARGE_ELEMENT_COUNT * sizeof(ObjVertex);
	obj.vertices = malloc(obj.verticesSize);
	assert(obj.vertices != NULL);
	for (int i = 0; i < LARGE_ELEMENT_COUNT; i++) {
		obj.vertices[i].x = (float)i;
		obj.vertices[i].y = (float)i;
		obj.vertices[i].z = (float)i;
	}

	/* Fill up texture coordinates. */
	obj.textureCoordsCount = LARGE_ELEMENT_COUNT;
	obj.textureCoordsSize = LARGE_ELEMENT_COUNT * sizeof(ObjTextureCoord);
	obj.textureCoords = malloc(obj.textureCoordsSize);
	assert(obj.textureCoords != NULL);
	for (int i = 0; i < LARGE_ELEMENT_COUNT; i++) {
		obj.textureCoords[i].u = (float)i;
		obj.textureCoords[i].v = (float)i;
	}

	/* Fill up normals. */
	obj.normalsCount = LARGE_ELEMENT_COUNT;
	obj.normalsSize = LARGE_ELEMENT_COUNT * sizeof(ObjNormal);
	obj.normals = malloc(obj.normalsSize);
	assert(obj.normals != NULL);
	for (int i = 0; i < LARGE_ELEMENT_COUNT; i++) {
		obj.normals[i].x = (float)i;
		obj.normals[i].y = (float)i;
		obj.normals[i].z = (float)i;
	}

	/* Fill up objects. */
	obj.objectsCount = LARGE_ELEMENT_COUNT;
	obj.objectsSize = LARGE_ELEMENT_COUNT * sizeof(ObjObject);
	obj.objects = malloc(obj.objectsSize);
	assert(obj.objects != NULL);
	for (int i = 0; i < LARGE_ELEMENT_COUNT; i++) {
		ObjObject *o = &obj.objects[i];

		o->name = calloc(17, sizeof(char));
		snprintf(o->name, 16, "Object %d", i);

		/* Fill up faces. */
		o->facesCount = SMALL_ELEMENT_COUNT;
		o->facesSize = SMALL_ELEMENT_COUNT * sizeof(ObjFace);
		o->faces = malloc(o->facesSize);
		assert(o->faces != NULL);
		for (int j = 0; j < SMALL_ELEMENT_COUNT; j++) {
			ObjFace *f = &o->faces[j];

			f->material = strdup("METAL");
			assert(f->material != NULL);

			f->verticesCount = SMALL_ELEMENT_COUNT;
			f->verticesSize = SMALL_ELEMENT_COUNT
				* sizeof(ObjFaceVertex);
			f->vertices = malloc(f->verticesSize);
			assert(f->vertices != NULL);
			for (int k = 0; k < SMALL_ELEMENT_COUNT; k++) {
				f->vertices[k].vertexIdx = k;
				f->vertices[k].textureIdx = k;
				f->vertices[k].normalIdx = k;
			}
		}
	}
	ObjFileFree(&obj);
}

int main()
{
	TestFree();
	return succeed;
}
