#pragma once

#include "Vector.h"

class Mesh
{
public:
	std::string name;
	std::string textureKey;
	vec3* vertex_array;
	unsigned int vertex_count;
	vec3* normal_array;
	unsigned int normal_count;
	vec2* uv_array;
	unsigned int uv_count;

	Mesh();
	~Mesh();

	void setPositions(const vec3* data, const int count);
	void setNormals(const vec3* data, const int count);
	void setUVs(const vec2* data, const int count);
	void setUVs(const vec3* data, const int count);
	void setScale(float scale);
	void print();
};