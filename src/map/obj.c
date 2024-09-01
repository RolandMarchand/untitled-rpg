#include "obj.h"
#include <assert.h>

/* Write the material library to 'out'. Return an error in case the action can't
 * be completed. */
static Error ObjWriteMaterialLibrary(FILE *out, ObjFile *obj)
{
	if (out == NULL || obj == NULL) {
		return ERR_NULL_REFERENCE;
	}

	if (obj->materialLibrary == NULL) {
		return ERR_OK;
	}

	int err = fprintf(out, "mtllib %s\n", obj->materialLibrary);
	if (unlikely(err == -1)) {
		PRINT_ERR("Unable to write OBJ to file\n");
		return ERR_FILE_WRITE_FAILURE;
	}

	return ERR_OK;
}

/* Write vertex data to 'out'. Return an error in case the action can't be
 * completed. */
static Error ObjWriteVertices(FILE *out, ObjFile *obj)
{
	if (out == NULL || obj == NULL) {
		return ERR_NULL_REFERENCE;
	}

	int err;
	/* Write vertex comment header. */
	if (obj->verticesCount <= 0) {
		err = fprintf(out, "# vertices\n");
		if (unlikely(err == -1)) {
			PRINT_ERR("Unable to write OBJ to file\n");
			return ERR_FILE_WRITE_FAILURE;
		}
	}

	for (size_t i = 0; i < obj->verticesCount; i++) {
		ObjVertex v = obj->vertices[i];
		err = fprintf(out, "v %.15g %.15g %.15g\n", v.x, v.y, v.z);
		if (unlikely(err == -1)) {
			PRINT_ERR("Unable to write OBJ to file\n");
			return ERR_FILE_WRITE_FAILURE;
		}
	}

	return ERR_OK;
}

/* Write texture coordinates to 'out'. Return an error in case the action can't
 * be completed. */
static Error ObjWriteTextureCoords(FILE *out, ObjFile *obj)
{
	assert(out != NULL);
	assert(obj != NULL);

	int err;
	/* Write texture coordinate comment header. */
	err = fprintf(out, "\n# texture coordinates\n");
	if (unlikely(err == -1)) {
		PRINT_ERR("Unable to write OBJ to file\n");
		return ERR_FILE_WRITE_FAILURE;
	}

	/* Write texture coordinate. */
	for (size_t i = 0; i < obj->textureCoordsCount; i++) {
		ObjTextureCoord vt = obj->textureCoords[i];
		err = fprintf(out, "vt %.15g %.15g\n", vt.u, vt.v);
		if (unlikely(err == -1)) {
			PRINT_ERR("Unable to write OBJ to file\n");
			return ERR_FILE_WRITE_FAILURE;
		}
	}

	return ERR_OK;
}

/* Write normals to 'out'. Return an error in case the action can't be
 * completed. */
static Error ObjWriteNormals(FILE *out, ObjFile *obj)
{
	assert(out != NULL);
	assert(obj != NULL);

	int err;
	/* Write face normal comment header. */
	err = fprintf(out, "\n# face normals\n");
	if (unlikely(err == -1)) {
		PRINT_ERR("Unable to write OBJ to file\n");
		return ERR_FILE_WRITE_FAILURE;
	}

	/* Write face normals. */
	for (size_t i = 0; i < obj->normalsCount; i++) {
		ObjNormal vn = obj->normals[i];
		err = fprintf(out, "vn %.15g %.15g %.15g\n", vn.x, vn.y, vn.z);
		if (unlikely(err == -1)) {
			PRINT_ERR("Unable to write OBJ to file\n");
			return ERR_FILE_WRITE_FAILURE;
		}
	}

	return ERR_OK;
}

/* Write a face vertex to 'out'. Return an error in case the action can't be
 * completed. */
static Error ObjWriteFaceVertex(FILE *out, ObjFaceVertex *fv)
{
	assert(out != NULL);
	assert(fv != NULL);

	int err;
	if (fv->vertexIdx == 0) {
		return ERR_INVALID_SYNTAX;
	}

	if (fv->textureIdx == 0) {
		if (fv->normalIdx == 0) {
			err = fprintf(out, " %zu", fv->vertexIdx);
		} else {
			err = fprintf(out, " %zu//%zu", fv->vertexIdx,
				      fv->normalIdx);
		}
	} else {
		if (fv->normalIdx == 0) {
			err = fprintf(out, " %zu/%zu", fv->vertexIdx,
				      fv->textureIdx);
		} else {
			err = fprintf(out, " %zu/%zu/%zu", fv->vertexIdx,
				      fv->textureIdx, fv->normalIdx);
		}
	}

	if (unlikely(err == -1)) {
		return ERR_FILE_WRITE_FAILURE;
	}

	return ERR_OK;
}

/* Write an object's faces to 'out'. Return an error in case the action can't be
 * completed. */
static Error ObjWriteFaces(FILE *out, ObjObject *o)
{
	assert(out != NULL);
	assert(o != NULL);

	int err;
	/* Write face normals. */
	for (size_t i = 0; i < o->facesCount; i++) {
		ObjFace f = o->faces[i];
		if (f.material != NULL) {
			/* Write face texture. */
			err = fprintf(out, "usemtl %s\n", f.material);
			if (unlikely(err == -1)) {
				return ERR_FILE_WRITE_FAILURE;
			}
		}

		/* Write face declaration. */
		err = fprintf(out, "f");
		if (unlikely(err == -1)) {
			return ERR_FILE_WRITE_FAILURE;
		}

		/* Write vertices. */
		for (size_t j = 0; j < f.verticesCount; j++) {
			ObjFaceVertex fv = f.vertices[j];
			if (ObjWriteFaceVertex(out, &fv) != ERR_OK) {
				return ERR_INVALID_SYNTAX;
			}
		}

		err = fprintf(out, "\n");
		if (unlikely(err == -1)) {
			return ERR_FILE_WRITE_FAILURE;
		}
	}

	return ERR_OK;
}

/* Write objects to 'out'. Return an error in case the action can't be
 * completed. */
static Error ObjWriteObjects(FILE *out, ObjFile *obj)
{
	assert(out != NULL);
	assert(obj != NULL);

	int err;
	/* Write object comment header. */
	err = fprintf(out, "\n# objects\n");
	if (unlikely(err == -1)) {
		PRINT_ERR("Unable to write OBJ to file\n");
		return ERR_FILE_WRITE_FAILURE;
	}

	/* Write objects. */
	for (size_t i = 0; i < obj->objectsCount; i++) {
		ObjObject o = obj->objects[i];

		/* Write object declaration. */
		if (o.name != NULL) {
			err = fprintf(out, "o %s\n", o.name);
			if (unlikely(err == -1)) {
				PRINT_ERR("Unable to write OBJ to file\n");
				return ERR_FILE_WRITE_FAILURE;
			}
		} else {
			err = fprintf(out, "o\n");
			if (unlikely(err == -1)) {
				PRINT_ERR("Unable to write OBJ to file\n");
				return ERR_FILE_WRITE_FAILURE;
			}
		}

		/* Write faces. */
		if (ObjWriteFaces(out, &o) != ERR_OK) {
			return ERR_FILE_WRITE_FAILURE;
		}

		/* Write newline. */
		err = fprintf(out, "\n");
		if (unlikely(err == -1)) {
			return ERR_FILE_WRITE_FAILURE;
		}
	}

	return ERR_OK;
}

Error ObjGenerateFile(FILE **fileOut, ObjFile *objIn, long *sizeOut)
{
	/* Preamble. */
	if (fileOut == NULL || objIn == NULL) {
		PRINT_ERR("Null reference exception in argument.\n");
		return ERR_NULL_REFERENCE;
	}

	int err;
	*fileOut = tmpfile();
	if (*fileOut == NULL) {
		PRINT_ERR("Unable to open temporary file.\n");
		return ERR_FILE_NOT_FOUND;
	}


	/* Write OBJ. */
	if ((err = ObjWriteMaterialLibrary(*fileOut, objIn)) != ERR_OK
	    || (err = ObjWriteVertices(*fileOut, objIn)) != ERR_OK
	    || (err = ObjWriteTextureCoords(*fileOut, objIn)) != ERR_OK
	    || (err = ObjWriteNormals(*fileOut, objIn)) != ERR_OK
	    || (err = ObjWriteObjects(*fileOut, objIn)) != ERR_OK) {
		fclose(*fileOut);
		return err;
	}

	/* Calculate size. */
	if (sizeOut != NULL) {
		err = fseek(*fileOut, 0, SEEK_END);
		if (err != 0) {
			fclose(*fileOut);
			return ERR_FILE_SEEK_FAILURE;
		}
		*sizeOut = ftell(*fileOut);
		if (*sizeOut == -1) {
			fclose(*fileOut);
			return ERR_FILE_SEEK_FAILURE;
		}
		err = fseek(*fileOut, 0, SEEK_SET);
		if (err != 0) {
			fclose(*fileOut);
			return ERR_FILE_SEEK_FAILURE;
		}
	}

	/* Clean context so it's ready to be used. */
	fflush(*fileOut);
	err = fseek(*fileOut, 0, SEEK_SET);
	if (err != 0) {
		fclose(*fileOut);
		return ERR_FILE_SEEK_FAILURE;
	}

	return ERR_OK;
}

void ObjFileFree(ObjFile *obj)
{
	free(obj->materialLibrary);
	free(obj->vertices);
	free(obj->textureCoords);
	free(obj->normals);

	for (size_t i = 0; i < obj->objectsCount; i++) {
		ObjObject o = obj->objects[i];
		free(o.name);
		for (size_t j = 0; j < o.facesCount; j++) {
			free(o.faces[j].material);
			free(o.faces[j].vertices);
		}
		free(o.faces);
	}

	free(obj->objects);
	
	obj->materialLibrary = NULL;
	obj->vertices = NULL;
	obj->textureCoords = NULL;
	obj->normals = NULL;
	obj->objects = NULL;
}
