#pragma once

#include "common.h"
#include "map.h"

#include <float.h>
#include <math.h>
#include <stdbool.h>

typedef struct ALIGN(16) Vector3 {
	float x, y, z;
} Vector3;

typedef struct ALIGN(32) Plane {
	Vector3 normal;
	float d;
} Plane;

/* Might need tweaking, may be overkill. */
static inline bool NearlyEqual(float a, float b, float epsilon)
{
	const float absA = fabsf(a);
	const float absB = fabsf(b);
	const float diff = fabsf(a - b);

	if (a == b) {
		return true;
	}
	if (a == 0 || b == 0 || (absA + absB < FLT_MIN)) {
		return diff < (epsilon * FLT_MIN);
	}
	return diff / fminf((absA + absB), FLT_MAX) < epsilon;
}

static inline bool LessThan(float a, float b, float epsilon)
{
	return (a < b) && !NearlyEqual(a, b, epsilon);
}

static inline bool GreaterThan(float a, float b, float epsilon)
{
	return (a > b) && !NearlyEqual(a, b, epsilon);
}

static inline Vector3 RightToLeft(Vector3 right)
{
	Vector3 left;
	left.x = right.x;
	left.y = right.z;
	left.z = -right.y;
	return left;
}

static inline float Dot(Vector3 left, Vector3 right)
{
	return left.x * right.x + left.y * right.y + left.z * right.z;
}

static inline Vector3 Cross(Vector3 left, Vector3 right)
{
	Vector3 out;
	out.x = left.y * right.z - left.z * right.y;
	out.y = left.z * right.x - left.x * right.z;
	out.z = left.x * right.y - left.y * right.x;
	return out;
}

static inline Vector3 Add(Vector3 v, float s)
{
	v.x += s;
	v.y += s;
	v.z += s;
	return v;
}

static inline Vector3 VAdd(Vector3 v1, Vector3 v2)
{
	v1.x += v2.x;
	v1.y += v2.y;
	v1.z += v2.z;
	return v1;
}

static inline Vector3 Sub(Vector3 v, float s)
{
	v.x -= s;
	v.y -= s;
	v.z -= s;
	return v;
}

static inline Vector3 VSub(Vector3 v1, Vector3 v2)
{
	v1.x -= v2.x;
	v1.y -= v2.y;
	v1.z -= v2.z;
	return v1;
}

static inline Vector3 Mult(Vector3 v, float s)
{
	v.x *= s;
	v.y *= s;
	v.z *= s;
	return v;
}

static inline Vector3 Div(Vector3 v, float s)
{
	v.x /= s;
	v.y /= s;
	v.z /= s;
	return v;
}

static inline float LengthSquared(Vector3 v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

static inline float Length(Vector3 v)
{
	return sqrtf(LengthSquared(v));
}

/* Not a perfectly accurate centroid, but all we need is a point inside of the
 * brush. */
static inline Vector3 GetCentroid(const MapBrush *in)
{
	Vector3 out = {0, 0, 0};
	for (int i = 0; i < in->facesCount; i++) {
		out.x += in->faces[i].points[POINT_1_X];
		out.x += in->faces[i].points[POINT_2_X];
		out.x += in->faces[i].points[POINT_3_X];
		out.y += in->faces[i].points[POINT_1_Y];
		out.y += in->faces[i].points[POINT_2_Y];
		out.y += in->faces[i].points[POINT_3_Y];
		out.z += in->faces[i].points[POINT_1_Z];
		out.z += in->faces[i].points[POINT_2_Z];
		out.z += in->faces[i].points[POINT_3_Z];
	}
	out.x /= (float)in->facesCount;
	out.y /= (float)in->facesCount;
	out.z /= (float)in->facesCount;
	return out;
}

static inline Vector3 Normalize(Vector3 v)
{
	float lengthSq = LengthSquared(v);
	if (NearlyEqual(lengthSq, 0, FLT_EPSILON)) {
		v.x = v.y = v.z = 0;
	} else {
		float length = sqrtf(lengthSq);
		v.x /= length;
		v.y /= length;
		v.z /= length;
	}
	return v;
}

static inline Plane GetPlane(Vector3 v1, Vector3 v2, Vector3 v3)
{
	Plane ret;

	Vector3 planeVector1 = VSub(v1, v3);
	Vector3 planeVector2 = VSub(v1, v2);

	ret.normal = Normalize(Cross(planeVector1, planeVector2));
	ret.d = Dot(ret.normal, v1);

	return ret;
}

static inline bool GetIntersection(Plane p1, Plane p2, Plane p3, Vector3 *out)
{
	float denom = Dot(Cross(p1.normal, p2.normal), p3.normal);

	if (NearlyEqual(denom, 0, FLT_EPSILON)) {
		return false;
	}

	if (out == NULL) {
		return true;
	}

	*out = Div(VAdd(VAdd(Mult(Cross(p2.normal, p3.normal), p1.d),
			 Mult(Cross(p3.normal, p1.normal), p2.d)),
			Mult(Cross(p1.normal, p2.normal), p3.d)), denom);

	return true;
}

static inline Plane MapFaceToPlane(const MapFace *face)
{
	Vector3 p1 = {
		face->points[POINT_1_X],
		face->points[POINT_1_Y],
		face->points[POINT_1_Z],
	};
	Vector3 p2 = {
		face->points[POINT_2_X],
		face->points[POINT_2_Y],
		face->points[POINT_2_Z],
	};
	Vector3 p3 = {
		face->points[POINT_3_X],
		face->points[POINT_3_Y],
		face->points[POINT_3_Z],
	};

	Plane ret;
	ret = GetPlane(p1, p2, p3);
	return ret;
}
