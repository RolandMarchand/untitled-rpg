#pragma once

#include "common.h"

#include <stdio.h>
#include <stdlib.h>

/* Structures to represent a basic OBJ file. The user is expected to manually
 * construct the OBJ. It is recommended to use calloc() instea of malloc() to
 * avoid issues with empty attributes. Two functions are provided: ObjFileFree()
 * to tree-walk free the OBJ, and ObjGenerateFile() to serialize it.  */

typedef struct ALIGN(16) ObjVertex {
	float x, y, z;
} ObjVertex;

typedef struct ALIGN(8) ObjTextureCoord {
	float u, v;
} ObjTextureCoord;

typedef struct ALIGN(16) ObjNormal {
	float x, y, z;
} ObjNormal;

typedef struct ALIGN(32) ObjFaceVertex {
	size_t vertexIdx;
	size_t textureIdx;
	size_t normalIdx;
} ObjFaceVertex;

typedef struct ALIGN(32) ObjFace {
	char *material;
	ObjFaceVertex *vertices;
	size_t verticesCount;
	size_t verticesSize;
} ObjFace;

typedef struct ALIGN(32) ObjObject {
	char *name;
	ObjFace *faces;
	size_t facesCount;
	size_t facesSize;
} ObjObject;

typedef struct ALIGN(128) ObjFile {
	char *materialLibrary;

	ObjVertex *vertices;
	size_t verticesCount;
	size_t verticesSize;

	ObjTextureCoord *textureCoords;
	size_t textureCoordsCount;
	size_t textureCoordsSize;

	ObjNormal *normals;
	size_t normalsCount;
	size_t normalsSize;

	ObjObject *objects;
	size_t objectsCount;
	size_t objectsSize;
} ObjFile;


void ObjFileFree(ObjFile *obj);
Error ObjGenerateFile(FILE **fileOut, ObjFile *objIn, long *sizeOut);
