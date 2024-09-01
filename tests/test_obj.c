#include "map/obj.h"

#include <assert.h>
#include <string.h>

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

void GetPolygonsObj(ObjFile *obj)
{
	obj->materialLibrary = strdup("solids.mtl");

	obj->verticesCount = 50;
	obj->verticesSize = obj->verticesCount * sizeof(ObjVertex);
	obj->vertices = calloc(obj->verticesCount, sizeof(ObjVertex));
	assert(obj->vertices != NULL);
	obj->vertices[0].x = 0.5773502691896258f;
	obj->vertices[0].y = 0.5773502691896258f;
	obj->vertices[0].z = 0.5773502691896258f;
	obj->vertices[1].x = -0.5773502691896258f;
	obj->vertices[1].y = -0.5773502691896258f;
	obj->vertices[1].z = 0.5773502691896258f;
	obj->vertices[2].x = -0.5773502691896258f;
	obj->vertices[2].y = 0.5773502691896258f;
	obj->vertices[2].z = -0.5773502691896258f;
	obj->vertices[3].x = 0.5773502691896258f;
	obj->vertices[3].y = -0.5773502691896258f;
	obj->vertices[3].z = -0.5773502691896258f;
	obj->vertices[4].x = 0.5773502691896258f;
	obj->vertices[4].y = 3.5773502691896257f;
	obj->vertices[4].z = 0.5773502691896258f;
	obj->vertices[5].x = 0.5773502691896258f;
	obj->vertices[5].y = 3.5773502691896257f;
	obj->vertices[5].z = -0.5773502691896258f;
	obj->vertices[6].x = 0.5773502691896258f;
	obj->vertices[6].y = 2.4226497308103743f;
	obj->vertices[6].z = 0.5773502691896258f;
	obj->vertices[7].x = 0.5773502691896258f;
	obj->vertices[7].y = 2.4226497308103743f;
	obj->vertices[7].z = -0.5773502691896258f;
	obj->vertices[8].x = -0.5773502691896258f;
	obj->vertices[8].y = 3.5773502691896257f;
	obj->vertices[8].z = 0.5773502691896258f;
	obj->vertices[9].x = -0.5773502691896258f;
	obj->vertices[9].y = 3.5773502691896257f;
	obj->vertices[9].z = -0.5773502691896258f;
	obj->vertices[10].x = -0.5773502691896258f;
	obj->vertices[10].y = 2.4226497308103743f;
	obj->vertices[10].z = 0.5773502691896258f;
	obj->vertices[11].x = -0.5773502691896258f;
	obj->vertices[11].y = 2.4226497308103743f;
	obj->vertices[11].z = -0.5773502691896258f;
	obj->vertices[12].x = 4.0f;
	obj->vertices[12].y = 0.0f;
	obj->vertices[12].z = 0.0f;
	obj->vertices[13].x = 2.0f;
	obj->vertices[13].y = 0.0f;
	obj->vertices[13].z = 0.0f;
	obj->vertices[14].x = 3.0f;
	obj->vertices[14].y = 1.0f;
	obj->vertices[14].z = 0.0f;
	obj->vertices[15].x = 3.0f;
	obj->vertices[15].y = -1.0f;
	obj->vertices[15].z = 0.0f;
	obj->vertices[16].x = 3.0f;
	obj->vertices[16].y = 0.0f;
	obj->vertices[16].z = 1.0f;
	obj->vertices[17].x = 3.0f;
	obj->vertices[17].y = 0.0f;
	obj->vertices[17].z = -1.0f;
	obj->vertices[18].x = 3.5773502691896257f;
	obj->vertices[18].y = 3.5773502691896257f;
	obj->vertices[18].z = 0.5773502691896258f;
	obj->vertices[19].x = 3.5773502691896257f;
	obj->vertices[19].y = 3.5773502691896257f;
	obj->vertices[19].z = -0.5773502691896258f;
	obj->vertices[20].x = 3.5773502691896257f;
	obj->vertices[20].y = 2.4226497308103743f;
	obj->vertices[20].z = 0.5773502691896258f;
	obj->vertices[21].x = 2.4226497308103743f;
	obj->vertices[21].y = 3.5773502691896257f;
	obj->vertices[21].z = 0.5773502691896258f;
	obj->vertices[22].x = 3.5773502691896257f;
	obj->vertices[22].y = 2.4226497308103743f;
	obj->vertices[22].z = -0.5773502691896258f;
	obj->vertices[23].x = 2.4226497308103743f;
	obj->vertices[23].y = 3.5773502691896257f;
	obj->vertices[23].z = -0.5773502691896258f;
	obj->vertices[24].x = 2.4226497308103743f;
	obj->vertices[24].y = 2.4226497308103743f;
	obj->vertices[24].z = 0.5773502691896258f;
	obj->vertices[25].x = 2.4226497308103743f;
	obj->vertices[25].y = 2.4226497308103743f;
	obj->vertices[25].z = -0.5773502691896258f;
	obj->vertices[26].x = 3.0f;
	obj->vertices[26].y = 3.35682208977309f;
	obj->vertices[26].z = 0.9341723589627159f;
	obj->vertices[27].x = 3.0f;
	obj->vertices[27].y = 3.35682208977309f;
	obj->vertices[27].z = -0.9341723589627159f;
	obj->vertices[28].x = 3.0f;
	obj->vertices[28].y = 2.64317791022691f;
	obj->vertices[28].z = 0.9341723589627159f;
	obj->vertices[29].x = 3.0f;
	obj->vertices[29].y = 2.64317791022691f;
	obj->vertices[29].z = -0.9341723589627159f;
	obj->vertices[30].x = 3.35682208977309f;
	obj->vertices[30].y = 3.9341723589627158f;
	obj->vertices[30].z = 0.0f;
	obj->vertices[31].x = 3.35682208977309f;
	obj->vertices[31].y = 2.0658276410372842f;
	obj->vertices[31].z = 0.0f;
	obj->vertices[32].x = 2.64317791022691f;
	obj->vertices[32].y = 3.9341723589627158f;
	obj->vertices[32].z = 0.0f;
	obj->vertices[33].x = 2.64317791022691f;
	obj->vertices[33].y = 2.0658276410372842f;
	obj->vertices[33].z = 0.0f;
	obj->vertices[34].x = 3.9341723589627158f;
	obj->vertices[34].y = 3.0f;
	obj->vertices[34].z = 0.35682208977308993f;
	obj->vertices[35].x = 2.0658276410372842f;
	obj->vertices[35].y = 3.0f;
	obj->vertices[35].z = 0.35682208977308993f;
	obj->vertices[36].x = 3.9341723589627158f;
	obj->vertices[36].y = 3.0f;
	obj->vertices[36].z = -0.35682208977308993f;
	obj->vertices[37].x = 2.0658276410372842f;
	obj->vertices[37].y = 3.0f;
	obj->vertices[37].z = -0.35682208977308993f;
	obj->vertices[38].x = 6.0f;
	obj->vertices[38].y = 2.0257311121191335f;
	obj->vertices[38].z = 0.85065080835204f;
	obj->vertices[39].x = 6.0f;
	obj->vertices[39].y = 2.0257311121191335f;
	obj->vertices[39].z = -0.85065080835204f;
	obj->vertices[40].x = 6.0f;
	obj->vertices[40].y = 0.9742688878808664f;
	obj->vertices[40].z = 0.85065080835204f;
	obj->vertices[41].x = 6.0f;
	obj->vertices[41].y = 0.9742688878808664f;
	obj->vertices[41].z = -0.85065080835204f;
	obj->vertices[42].x = 6.525731112119134f;
	obj->vertices[42].y = 2.35065080835204f;
	obj->vertices[42].z = 0.0f;
	obj->vertices[43].x = 6.525731112119134f;
	obj->vertices[43].y = 0.64934919164796f;
	obj->vertices[43].z = 0.0f;
	obj->vertices[44].x = 5.474268887880866f;
	obj->vertices[44].y = 2.35065080835204f;
	obj->vertices[44].z = 0.0f;
	obj->vertices[45].x = 5.474268887880866f;
	obj->vertices[45].y = 0.64934919164796f;
	obj->vertices[45].z = 0.0f;
	obj->vertices[46].x = 6.85065080835204f;
	obj->vertices[46].y = 1.5f;
	obj->vertices[46].z = 0.5257311121191336f;
	obj->vertices[47].x = 6.85065080835204f;
	obj->vertices[47].y = 1.5f;
	obj->vertices[47].z = -0.5257311121191336f;
	obj->vertices[48].x = 5.14934919164796f;
	obj->vertices[48].y = 1.5f;
	obj->vertices[48].z = 0.5257311121191336f;
	obj->vertices[49].x = 5.14934919164796f;
	obj->vertices[49].y = 1.5f;
	obj->vertices[49].z = -0.5257311121191336f;

	obj->objectsCount = 5;
	obj->objectsSize = obj->objectsCount * sizeof(ObjObject);
	obj->objects = calloc(obj->objectsCount, sizeof(ObjObject));

	ObjObject *tetra = &obj->objects[0];
	ObjObject *hexa = &obj->objects[1];
	ObjObject *octa = &obj->objects[2];
	ObjObject *dodeca = &obj->objects[3];
	ObjObject *icosa = &obj->objects[4];

	tetra->name = strdup("Tetrahedron");
	hexa->name = strdup("Hexahedron");
	octa->name = strdup("Octahedron");
	dodeca->name = strdup("Dodecahedron");
	icosa->name = strdup("Icosahedron");

	tetra->facesCount = 4;
	hexa->facesCount = 6;
	octa->facesCount = 8;
	dodeca->facesCount = 12;
	icosa->facesCount = 20;

	tetra->facesSize = tetra->facesCount * sizeof(ObjFace);
	hexa->facesSize = hexa->facesCount * sizeof(ObjFace);
	octa->facesSize = octa->facesCount * sizeof(ObjFace);
	dodeca->facesSize = dodeca->facesCount * sizeof(ObjFace);
	icosa->facesSize = icosa->facesCount * sizeof(ObjFace);

	tetra->faces = calloc(tetra->facesCount, sizeof(ObjFace));
	hexa->faces = calloc(hexa->facesCount, sizeof(ObjFace));
	octa->faces = calloc(octa->facesCount, sizeof(ObjFace));
	dodeca->faces = calloc(dodeca->facesCount, sizeof(ObjFace));
	icosa->faces = calloc(icosa->facesCount, sizeof(ObjFace));

	tetra->faces[0].material = strdup("Red");
	hexa->faces[0].material = strdup("Green");
	octa->faces[0].material = strdup("Blue");
	dodeca->faces[0].material = strdup("Orange");
	icosa->faces[0].material = strdup("Purple");

	for (int i = 0; i < tetra->facesCount; i++) {
		tetra->faces[i].verticesCount = 3;
		tetra->faces[i].verticesSize = tetra->faces[i].verticesCount
			* sizeof(ObjFaceVertex);
		tetra->faces[i].vertices = calloc(tetra->faces[i].verticesCount,
						  sizeof(ObjFaceVertex));
	}

	for (int i = 0; i < hexa->facesCount; i++) {
		hexa->faces[i].verticesCount = 4;
		hexa->faces[i].verticesSize = hexa->faces[i].verticesCount
			* sizeof(ObjFaceVertex);
		hexa->faces[i].vertices = calloc(hexa->faces[i].verticesCount,
						  sizeof(ObjFaceVertex));
	}

	for (int i = 0; i < octa->facesCount; i++) {
		octa->faces[i].verticesCount = 3;
		octa->faces[i].verticesSize = octa->faces[i].verticesCount
			* sizeof(ObjFaceVertex);
		octa->faces[i].vertices = calloc(octa->faces[i].verticesCount,
						  sizeof(ObjFaceVertex));
	}

	for (int i = 0; i < dodeca->facesCount; i++) {
		dodeca->faces[i].verticesCount = 5;
		dodeca->faces[i].verticesSize = dodeca->faces[i].verticesCount
			* sizeof(ObjFaceVertex);
		dodeca->faces[i].vertices = calloc(
			dodeca->faces[i].verticesCount, sizeof(ObjFaceVertex));
	}

	for (int i = 0; i < icosa->facesCount; i++) {
		icosa->faces[i].verticesCount = 3;
		icosa->faces[i].verticesSize = icosa->faces[i].verticesCount
			* sizeof(ObjFaceVertex);
		icosa->faces[i].vertices = calloc(icosa->faces[i].verticesCount,
						  sizeof(ObjFaceVertex));
	}

	tetra->faces[0].vertices[0].vertexIdx = 1;
	tetra->faces[0].vertices[1].vertexIdx = 2;
	tetra->faces[0].vertices[2].vertexIdx = 4;
	tetra->faces[1].vertices[0].vertexIdx = 1;
	tetra->faces[1].vertices[1].vertexIdx = 3;
	tetra->faces[1].vertices[2].vertexIdx = 2;
	tetra->faces[2].vertices[0].vertexIdx = 1;
	tetra->faces[2].vertices[1].vertexIdx = 4;
	tetra->faces[2].vertices[2].vertexIdx = 3;
	tetra->faces[3].vertices[0].vertexIdx = 2;
	tetra->faces[3].vertices[1].vertexIdx = 3;
	tetra->faces[3].vertices[2].vertexIdx = 4;

	hexa->faces[0].vertices[0].vertexIdx = 5;
	hexa->faces[0].vertices[1].vertexIdx = 6;
	hexa->faces[0].vertices[2].vertexIdx = 10;
	hexa->faces[0].vertices[3].vertexIdx = 9;
	hexa->faces[1].vertices[0].vertexIdx = 5;
	hexa->faces[1].vertices[1].vertexIdx = 7;
	hexa->faces[1].vertices[2].vertexIdx = 8;
	hexa->faces[1].vertices[3].vertexIdx = 6;
	hexa->faces[2].vertices[0].vertexIdx = 5;
	hexa->faces[2].vertices[1].vertexIdx = 9;
	hexa->faces[2].vertices[2].vertexIdx = 11;
	hexa->faces[2].vertices[3].vertexIdx = 7;
	hexa->faces[3].vertices[0].vertexIdx = 6;
	hexa->faces[3].vertices[1].vertexIdx = 8;
	hexa->faces[3].vertices[2].vertexIdx = 12;
	hexa->faces[3].vertices[3].vertexIdx = 10;
	hexa->faces[4].vertices[0].vertexIdx = 7;
	hexa->faces[4].vertices[1].vertexIdx = 11;
	hexa->faces[4].vertices[2].vertexIdx = 12;
	hexa->faces[4].vertices[3].vertexIdx = 8;
	hexa->faces[5].vertices[0].vertexIdx = 9;
	hexa->faces[5].vertices[1].vertexIdx = 10;
	hexa->faces[5].vertices[2].vertexIdx = 12;
	hexa->faces[5].vertices[3].vertexIdx = 11;

	octa->faces[0].vertices[0].vertexIdx = 13;
	octa->faces[0].vertices[1].vertexIdx = 15;
	octa->faces[0].vertices[2].vertexIdx = 17;
	octa->faces[1].vertices[0].vertexIdx = 13;
	octa->faces[1].vertices[1].vertexIdx = 16;
	octa->faces[1].vertices[2].vertexIdx = 18;
	octa->faces[2].vertices[0].vertexIdx = 13;
	octa->faces[2].vertices[1].vertexIdx = 17;
	octa->faces[2].vertices[2].vertexIdx = 16;
	octa->faces[3].vertices[0].vertexIdx = 13;
	octa->faces[3].vertices[1].vertexIdx = 18;
	octa->faces[3].vertices[2].vertexIdx = 15;
	octa->faces[4].vertices[0].vertexIdx = 14;
	octa->faces[4].vertices[1].vertexIdx = 15;
	octa->faces[4].vertices[2].vertexIdx = 18;
	octa->faces[5].vertices[0].vertexIdx = 14;
	octa->faces[5].vertices[1].vertexIdx = 16;
	octa->faces[5].vertices[2].vertexIdx = 17;
	octa->faces[6].vertices[0].vertexIdx = 14;
	octa->faces[6].vertices[1].vertexIdx = 17;
	octa->faces[6].vertices[2].vertexIdx = 15;
	octa->faces[7].vertices[0].vertexIdx = 14;
	octa->faces[7].vertices[1].vertexIdx = 18;
	octa->faces[7].vertices[2].vertexIdx = 16;

	dodeca->faces[0].vertices[0].vertexIdx = 19;
	dodeca->faces[0].vertices[1].vertexIdx = 27;
	dodeca->faces[0].vertices[2].vertexIdx = 29;
	dodeca->faces[0].vertices[3].vertexIdx = 21;
	dodeca->faces[0].vertices[4].vertexIdx = 35;
	dodeca->faces[1].vertices[0].vertexIdx = 19;
	dodeca->faces[1].vertices[1].vertexIdx = 35;
	dodeca->faces[1].vertices[2].vertexIdx = 37;
	dodeca->faces[1].vertices[3].vertexIdx = 20;
	dodeca->faces[1].vertices[4].vertexIdx = 31;
	dodeca->faces[2].vertices[0].vertexIdx = 19;
	dodeca->faces[2].vertices[1].vertexIdx = 31;
	dodeca->faces[2].vertices[2].vertexIdx = 33;
	dodeca->faces[2].vertices[3].vertexIdx = 22;
	dodeca->faces[2].vertices[4].vertexIdx = 27;
	dodeca->faces[3].vertices[0].vertexIdx = 20;
	dodeca->faces[3].vertices[1].vertexIdx = 28;
	dodeca->faces[3].vertices[2].vertexIdx = 24;
	dodeca->faces[3].vertices[3].vertexIdx = 33;
	dodeca->faces[3].vertices[4].vertexIdx = 31;
	dodeca->faces[4].vertices[0].vertexIdx = 20;
	dodeca->faces[4].vertices[1].vertexIdx = 37;
	dodeca->faces[4].vertices[2].vertexIdx = 23;
	dodeca->faces[4].vertices[3].vertexIdx = 30;
	dodeca->faces[4].vertices[4].vertexIdx = 28;
	dodeca->faces[5].vertices[0].vertexIdx = 21;
	dodeca->faces[5].vertices[1].vertexIdx = 29;
	dodeca->faces[5].vertices[2].vertexIdx = 25;
	dodeca->faces[5].vertices[3].vertexIdx = 34;
	dodeca->faces[5].vertices[4].vertexIdx = 32;
	dodeca->faces[6].vertices[0].vertexIdx = 21;
	dodeca->faces[6].vertices[1].vertexIdx = 32;
	dodeca->faces[6].vertices[2].vertexIdx = 23;
	dodeca->faces[6].vertices[3].vertexIdx = 37;
	dodeca->faces[6].vertices[4].vertexIdx = 35;
	dodeca->faces[7].vertices[0].vertexIdx = 22;
	dodeca->faces[7].vertices[1].vertexIdx = 33;
	dodeca->faces[7].vertices[2].vertexIdx = 24;
	dodeca->faces[7].vertices[3].vertexIdx = 38;
	dodeca->faces[7].vertices[4].vertexIdx = 36;
	dodeca->faces[8].vertices[0].vertexIdx = 22;
	dodeca->faces[8].vertices[1].vertexIdx = 36;
	dodeca->faces[8].vertices[2].vertexIdx = 25;
	dodeca->faces[8].vertices[3].vertexIdx = 29;
	dodeca->faces[8].vertices[4].vertexIdx = 27;
	dodeca->faces[9].vertices[0].vertexIdx = 23;
	dodeca->faces[9].vertices[1].vertexIdx = 32;
	dodeca->faces[9].vertices[2].vertexIdx = 34;
	dodeca->faces[9].vertices[3].vertexIdx = 26;
	dodeca->faces[9].vertices[4].vertexIdx = 30;
	dodeca->faces[10].vertices[0].vertexIdx = 24;
	dodeca->faces[10].vertices[1].vertexIdx = 28;
	dodeca->faces[10].vertices[2].vertexIdx = 30;
	dodeca->faces[10].vertices[3].vertexIdx = 26;
	dodeca->faces[10].vertices[4].vertexIdx = 38;
	dodeca->faces[11].vertices[0].vertexIdx = 25;
	dodeca->faces[11].vertices[1].vertexIdx = 36;
	dodeca->faces[11].vertices[2].vertexIdx = 38;
	dodeca->faces[11].vertices[3].vertexIdx = 26;
	dodeca->faces[11].vertices[4].vertexIdx = 34;

	icosa->faces[0].vertices[0].vertexIdx = 39;
	icosa->faces[0].vertices[1].vertexIdx = 41;
	icosa->faces[0].vertices[2].vertexIdx = 47;
	icosa->faces[1].vertices[0].vertexIdx = 39;
	icosa->faces[1].vertices[1].vertexIdx = 43;
	icosa->faces[1].vertices[2].vertexIdx = 45;
	icosa->faces[2].vertices[0].vertexIdx = 39;
	icosa->faces[2].vertices[1].vertexIdx = 45;
	icosa->faces[2].vertices[2].vertexIdx = 49;
	icosa->faces[3].vertices[0].vertexIdx = 39;
	icosa->faces[3].vertices[1].vertexIdx = 47;
	icosa->faces[3].vertices[2].vertexIdx = 43;
	icosa->faces[4].vertices[0].vertexIdx = 39;
	icosa->faces[4].vertices[1].vertexIdx = 49;
	icosa->faces[4].vertices[2].vertexIdx = 41;
	icosa->faces[5].vertices[0].vertexIdx = 40;
	icosa->faces[5].vertices[1].vertexIdx = 42;
	icosa->faces[5].vertices[2].vertexIdx = 50;
	icosa->faces[6].vertices[0].vertexIdx = 40;
	icosa->faces[6].vertices[1].vertexIdx = 43;
	icosa->faces[6].vertices[2].vertexIdx = 48;
	icosa->faces[7].vertices[0].vertexIdx = 40;
	icosa->faces[7].vertices[1].vertexIdx = 45;
	icosa->faces[7].vertices[2].vertexIdx = 43;
	icosa->faces[8].vertices[0].vertexIdx = 40;
	icosa->faces[8].vertices[1].vertexIdx = 48;
	icosa->faces[8].vertices[2].vertexIdx = 42;
	icosa->faces[9].vertices[0].vertexIdx = 40;
	icosa->faces[9].vertices[1].vertexIdx = 50;
	icosa->faces[9].vertices[2].vertexIdx = 45;
	icosa->faces[10].vertices[0].vertexIdx = 41;
	icosa->faces[10].vertices[1].vertexIdx = 44;
	icosa->faces[10].vertices[2].vertexIdx = 47;
	icosa->faces[11].vertices[0].vertexIdx = 41;
	icosa->faces[11].vertices[1].vertexIdx = 46;
	icosa->faces[11].vertices[2].vertexIdx = 44;
	icosa->faces[12].vertices[0].vertexIdx = 41;
	icosa->faces[12].vertices[1].vertexIdx = 49;
	icosa->faces[12].vertices[2].vertexIdx = 46;
	icosa->faces[13].vertices[0].vertexIdx = 42;
	icosa->faces[13].vertices[1].vertexIdx = 44;
	icosa->faces[13].vertices[2].vertexIdx = 46;
	icosa->faces[14].vertices[0].vertexIdx = 42;
	icosa->faces[14].vertices[1].vertexIdx = 46;
	icosa->faces[14].vertices[2].vertexIdx = 50;
	icosa->faces[15].vertices[0].vertexIdx = 42;
	icosa->faces[15].vertices[1].vertexIdx = 48;
	icosa->faces[15].vertices[2].vertexIdx = 44;
	icosa->faces[16].vertices[0].vertexIdx = 43;
	icosa->faces[16].vertices[1].vertexIdx = 47;
	icosa->faces[16].vertices[2].vertexIdx = 48;
	icosa->faces[17].vertices[0].vertexIdx = 44;
	icosa->faces[17].vertices[1].vertexIdx = 48;
	icosa->faces[17].vertices[2].vertexIdx = 47;
	icosa->faces[18].vertices[0].vertexIdx = 45;
	icosa->faces[18].vertices[1].vertexIdx = 50;
	icosa->faces[18].vertices[2].vertexIdx = 49;
	icosa->faces[19].vertices[0].vertexIdx = 46;
	icosa->faces[19].vertices[1].vertexIdx = 49;
	icosa->faces[19].vertices[2].vertexIdx = 50;
}

void GetTestObj(ObjFile *obj)
{
	obj->materialLibrary = strdup("SHINY_METAL");
	assert(obj->materialLibrary != NULL);

	/* Fill up vertices. */
	obj->verticesCount = LARGE_ELEMENT_COUNT;
	obj->verticesSize = LARGE_ELEMENT_COUNT * sizeof(ObjVertex);
	obj->vertices = malloc(obj->verticesSize);
	assert(obj->vertices != NULL);
	for (int i = 0; i < LARGE_ELEMENT_COUNT; i++) {
		obj->vertices[i].x = (float)i;
		obj->vertices[i].y = (float)i;
		obj->vertices[i].z = (float)i;
	}

	/* Fill up texture coordinates. */
	obj->textureCoordsCount = LARGE_ELEMENT_COUNT;
	obj->textureCoordsSize = LARGE_ELEMENT_COUNT * sizeof(ObjTextureCoord);
	obj->textureCoords = malloc(obj->textureCoordsSize);
	assert(obj->textureCoords != NULL);
	for (int i = 0; i < LARGE_ELEMENT_COUNT; i++) {
		obj->textureCoords[i].u = (float)i;
		obj->textureCoords[i].v = (float)i;
	}

	/* Fill up normals. */
	obj->normalsCount = LARGE_ELEMENT_COUNT;
	obj->normalsSize = LARGE_ELEMENT_COUNT * sizeof(ObjNormal);
	obj->normals = malloc(obj->normalsSize);
	assert(obj->normals != NULL);
	for (int i = 0; i < LARGE_ELEMENT_COUNT; i++) {
		obj->normals[i].x = (float)i;
		obj->normals[i].y = (float)i;
		obj->normals[i].z = (float)i;
	}

	/* Fill up objects. */
	obj->objectsCount = LARGE_ELEMENT_COUNT;
	obj->objectsSize = LARGE_ELEMENT_COUNT * sizeof(ObjObject);
	obj->objects = malloc(obj->objectsSize);
	assert(obj->objects != NULL);
	for (int i = 0; i < LARGE_ELEMENT_COUNT; i++) {
		ObjObject *o = &obj->objects[i];

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
				/* + 1 because vertexIdx of 0 is invalid. */
				f->vertices[k].vertexIdx = k + 1;
				f->vertices[k].textureIdx = k + 1;
				f->vertices[k].normalIdx = k + 1;
			}
		}
	}
}

void TestFree()
{
	ObjFile obj;
	GetTestObj(&obj);
	ObjFileFree(&obj);
}

void TestFile()
{
	ObjFile obj;
	GetTestObj(&obj);
	FILE *out;
	long outSize;
	Error err = ObjGenerateFile(&out, &obj, &outSize);
	assert(err == ERR_OK);
	char *fileContents = calloc(outSize + 1, sizeof(char));
	fread(fileContents, outSize, sizeof(char), out);
	ObjFileFree(&obj);
	free(fileContents);
}

void TestPolygons()
{
	ObjFile obj;
	GetPolygonsObj(&obj);
	FILE *out;
	long outSize;
	Error err = ObjGenerateFile(&out, &obj, &outSize);
	assert(err == ERR_OK);
	char *fileContents = calloc(outSize + 1, sizeof(char));
	fread(fileContents, outSize, sizeof(char), out);
	ObjFileFree(&obj);
	free(fileContents);
}

int main()
{
	TestFree();
	TestFile();
	TestPolygons();
	return succeed;
}
