#include <iostream>
#include "Transform.h"

Transform::Transform()
{
	scale.setAll(1);
}

vec3 Transform::getForward()
{
	vec3 forward(0, 0, 1);

	forward = rotateVector(forward, *this);
	forward.x = -forward.x;
	forward.y = 0;

	return forward;
}

vec3 Transform::getRight()
{
	return -cross(getForward(), vec3(0, 1, 0)).normalized();
}

vec3 Transform::getUp()
{
	return cross(getForward(), getRight()).normalized();
}

vec3 expressVertexInNewAxes(vec3 vertex, vec3 p, vec3 q, vec3 r)
{
	p.normalize();
	q.normalize();
	r.normalize();
			
	vec3 newVertex;
			
	newVertex.x = vertex.x * p.x + vertex.y * q.x + vertex.z * r.x;
	newVertex.y = vertex.x * p.y + vertex.y * q.y + vertex.z * r.y;
	newVertex.z = vertex.x * p.z + vertex.y * q.z + vertex.z * r.z;

	return newVertex;
}

vec3 rotateVertexAroundZ (vec3 vertex,Transform worldTransform)
{
	vec3 xAxis(1, 0, 0);
	vec3 yAxis(0, 1, 0);
	vec3 zAxis(0, 0, 1);
	
	xAxis.x = cos(worldTransform.rotation.z);
	xAxis.y = sin(worldTransform.rotation.z);
	xAxis.z = 0;
	yAxis.x = cos(worldTransform.rotation.z + PI/2);
	yAxis.y = sin(worldTransform.rotation.z + PI/2);
	yAxis.z = 0;
	zAxis.x = 0;
	zAxis.y = 0;
	zAxis.z = 1;

	return expressVertexInNewAxes(vertex, xAxis, yAxis, zAxis);
}

vec3 rotateVertexAroundY (vec3 vertex, Transform worldTransform)
{
	vec3 xAxis(1, 0, 0);
	vec3 yAxis(0, 1, 0);
	vec3 zAxis(0, 0, 1);

	xAxis.x = cos(worldTransform.rotation.y);
	xAxis.y = 0;
	xAxis.z = sin(worldTransform.rotation.y);
	yAxis.x = 0;
	yAxis.y = 1;
	yAxis.z = 0;
	zAxis.x = -sin(worldTransform.rotation.y);
	zAxis.y = 0;
	zAxis.z = cos(worldTransform.rotation.y);

	return expressVertexInNewAxes(vertex, xAxis, yAxis, zAxis);
}

vec3 rotateVertexAroundX (vec3 vertex,Transform worldTransform)
{
	vec3 xAxis(1, 0, 0);
	vec3 yAxis(0, 1, 0);
	vec3 zAxis(0, 0, 1);

	xAxis.x = 1;
	xAxis.y = 0;
	xAxis.z = 0;
	yAxis.x = 0;
	yAxis.y = cos(worldTransform.rotation.x);
	yAxis.z = -sin(worldTransform.rotation.x);
	zAxis.x = 0;
	zAxis.y = sin(worldTransform.rotation.x);
	zAxis.z = cos(worldTransform.rotation.x);

	return expressVertexInNewAxes(vertex, xAxis, yAxis, zAxis);
}

vec3 transformVertex(vec3 vertex, Transform worldTransform)
{
	vertex = rotateVertexAroundY(vertex, worldTransform);
	vertex = rotateVertexAroundX(vertex, worldTransform);
	vertex = rotateVertexAroundZ(vertex, worldTransform);

	vertex.x = vertex.x * worldTransform.scale.x;
	vertex.y = vertex.y * worldTransform.scale.y;
	vertex.z = vertex.z * worldTransform.scale.z;
	
	vertex.x += worldTransform.position.x;
	vertex.y += worldTransform.position.y;
	vertex.z += worldTransform.position.z;

	return vertex;
}

vec3 rotateVector(vec3 vector, Transform worldTransform)
{
	vector = rotateVertexAroundY(vector, worldTransform);
	vector = rotateVertexAroundX(vector, worldTransform);
	vector = rotateVertexAroundZ(vector, worldTransform);

	return vector;
}