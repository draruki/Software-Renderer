#include "Mesh.h"

Mesh::Mesh()
{
	name = "";
	textureKey = "";
	vertex_array = NULL;
	normal_array = NULL;
	uv_array = NULL;
	vertex_count = 0;
	normal_count = 0;
	uv_count = 0;
}

Mesh::~Mesh()
{
	if (vertex_array != NULL)
		delete[] vertex_array;

	if (normal_array != NULL)
		delete[] normal_array;
	
	if (uv_array != NULL)
		delete[] uv_array;
}

void Mesh::setPositions(const vec3* data, const int count)
{
	if (vertex_array != NULL)
		delete[] vertex_array;

	vertex_array = new vec3[count];

	for (int i = 0; i < count; ++i)
	{
		vertex_array[i].x = data[i].x;
		vertex_array[i].y = data[i].y;
		vertex_array[i].z = data[i].z;
	}

	vertex_count = count;
}

void Mesh::setScale(float scale)
{
	for (int i = 0; i < vertex_count; ++i)
	{
		vertex_array[i] *= scale;
	}
}

void Mesh::setNormals(const vec3* data, const int count)
{
	if (normal_array != NULL)
		delete[] normal_array;

	normal_array = new vec3[count];

	for (int i = 0; i < count; ++i)
	{
		normal_array[i].x = data[i].x;
		normal_array[i].y = data[i].y;
		normal_array[i].z = data[i].z;
	}

	normal_count = count;
}

void Mesh::setUVs(const vec2* data, const int count)
{
	if (uv_array != NULL)
		delete[] uv_array;

	uv_array = new vec2[count];

	for (int i = 0; i < count; ++i)
	{
		uv_array[i].x = data[i].x;
		uv_array[i].y = data[i].y;
	}

	uv_count = count;
}

void Mesh::setUVs(const vec3* data, const int count)
{
	if (uv_array != NULL)
		delete[] uv_array;

	uv_array = new vec2[count];

	for (int i = 0; i < count; ++i)
	{
		uv_array[i].x = data[i].x;
		uv_array[i].y = data[i].y;
	}

	uv_count = count;
}

void Mesh::print()
{
	printf("mesh %s has %d vertices, %d normals and %d uvs\n", name.c_str(), vertex_count, normal_count, uv_count);
}