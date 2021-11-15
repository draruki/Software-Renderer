#pragma once

#include "Vector.h"
#include "MathUtils.h"

struct Transform
{
	Transform();

	vec3 position;
	vec3 rotation;
	vec3 scale;

	vec3 getForward();
	vec3 getRight();
	vec3 getUp();
};

vec3 expressVertexInNewAxes(vec3 vertex, vec3 p, vec3 q, vec3 r);
vec3 rotateVertexAroundZ (vec3 vertex,Transform worldTransform);
vec3 rotateVertexAroundY (vec3 vertex, Transform worldTransform);
vec3 rotateVertexAroundX (vec3 vertex,Transform worldTransform);
vec3 transformVertex(vec3 vertex, Transform worldTransform);
vec3 rotateVector(vec3 vector, Transform worldTransform);