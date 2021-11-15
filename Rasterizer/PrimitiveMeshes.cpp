#include "PrimitiveMeshes.h"

namespace PrimitiveMeshes
{
	Mesh* Cube()
	{
		const vec3 vertices[] = {
			vec3(-0.5f,  0.5f,  0.5f),
			vec3(-0.5f, -0.5f,  0.5f),
			vec3( 0.5f, -0.5f,  0.5f),
			vec3(-0.5f,  0.5f,  0.5f), 
			vec3(0.5f, -0.5f, 0.5f),  
			vec3(0.5f,  0.5f,  0.5f),  

			vec3(0.5f, -0.5f, -0.5f),
			vec3(-0.5f, -0.5f, -0.5f),  
			vec3(-0.5f,  0.5f, -0.5f), 
			vec3(0.5f,  0.5f, -0.5f),
			vec3(0.5f, -0.5f, -0.5f),
			vec3(-0.5f,  0.5f, -0.5f),

			vec3(-0.5f,  0.5f,  -0.5f), 
			vec3(-0.5f,  0.5f, 0.5f), 
			vec3(0.5f,  0.5f, 0.5f), 
			vec3(-0.5f,  0.5f,  -0.5f), 
			vec3(0.5f,  0.5f, 0.5f), 
			vec3(0.5f,  0.5f,  -0.5f),

			vec3(0.5f, -0.5f, 0.5f),
			vec3(-0.5f, -0.5f, 0.5f),
			vec3(-0.5f, -0.5f,  -0.5f),
			vec3(0.5f, -0.5f,  -0.5f),
			vec3(0.5f, -0.5f, 0.5f),
			vec3(-0.5f, -0.5f,  -0.5f),

			vec3(-0.5f, -0.5f,  -0.5f),
			vec3(-0.5f, -0.5f, 0.5f),
			vec3(-0.5f,  0.5f, 0.5f),
			vec3(-0.5f,  0.5f,  -0.5f),
			vec3(-0.5f, -0.5f,  -0.5f),
			vec3(-0.5f,  0.5f, 0.5f),

			vec3(0.5f,  0.5f, 0.5f),
			vec3(0.5f, -0.5f, 0.5f),
			vec3(0.5f, -0.5f,  -0.5f),
			vec3(0.5f,  0.5f, 0.5f),
			vec3(0.5f, -0.5f,  -0.5f),
			vec3(0.5f,  0.5f,  -0.5f),
		};
		const vec3 normals[] = {
			vec3(0, 0, 1),
			vec3(0, 0, 1),
			vec3(0, 0, 1),
			vec3(0, 0, 1),
			vec3(0, 0, 1),
			vec3(0, 0, 1),

			vec3(0, 0, -1),
			vec3(0, 0, -1),
			vec3(0, 0, -1),
			vec3(0, 0, -1),
			vec3(0, 0, -1),
			vec3(0, 0, -1),

			vec3(0, 1, 0),
			vec3(0, 1, 0),
			vec3(0, 1, 0),
			vec3(0, 1, 0),
			vec3(0, 1, 0),
			vec3(0, 1, 0),

			vec3(0, -1, 0),
			vec3(0, -1, 0),
			vec3(0, -1, 0),
			vec3(0, -1, 0),
			vec3(0, -1, 0),
			vec3(0, -1, 0),

			vec3(-1, 0, 0),
			vec3(-1, 0, 0),
			vec3(-1, 0, 0),
			vec3(-1, 0, 0),
			vec3(-1, 0, 0),
			vec3(-1, 0, 0),

			vec3(1, 0, 0),
			vec3(1, 0, 0),
			vec3(1, 0, 0),
			vec3(1, 0, 0),
			vec3(1, 0, 0),
			vec3(1, 0, 0)
		};
		const vec2 uvs[] = {
			vec2(0, 1),
			vec2(0, 0),
			vec2(1, 0),
			vec2(0, 1),
			vec2(1, 0),
			vec2(1, 1),

			vec2(0, 1),
			vec2(0, 0),
			vec2(1, 0),
			vec2(0, 1),
			vec2(1, 0),
			vec2(1, 1),

			vec2(0, 1),
			vec2(0, 0),
			vec2(1, 0),
			vec2(0, 1),
			vec2(1, 0),
			vec2(1, 1),

			vec2(0, 1),
			vec2(0, 0),
			vec2(1, 0),
			vec2(0, 1),
			vec2(1, 0),
			vec2(1, 1),

			vec2(0, 1),
			vec2(0, 0),
			vec2(1, 0),
			vec2(0, 1),
			vec2(1, 0),
			vec2(1, 1),

			vec2(0, 1),
			vec2(0, 0),
			vec2(1, 0),
			vec2(0, 1),
			vec2(1, 0),
			vec2(1, 1),
		};

		Mesh* mesh = new Mesh();
		mesh->setPositions(vertices, sizeof(vertices) / sizeof(vec3));
		mesh->setNormals(normals, sizeof(normals) / sizeof(vec3));
		mesh->setUVs(uvs, sizeof(uvs) / sizeof(vec2));

		return mesh;
	}

	Mesh* Cubeold()
	{
		vec3 vertices[] =
		{
			vec3(-0.5f,  0.5f,  0.5f),
			vec3(-0.5f, -0.5f,  0.5f),
			vec3(0.5f, -0.5f,  0.5f),
			vec3(-0.5f,  0.5f,  0.5f),
			vec3(0.5f, -0.5f,  0.5f),
			vec3(0.5f,  0.5f,  0.5f),

			vec3(-0.5f,  0.5f, -0.5f),
			vec3(-0.5f, -0.5f, -0.5f) ,
			vec3(0.5f, -0.5f, -0.5f),
			vec3(-0.5f,  0.5f, -0.5f),
			vec3(0.5f, -0.5f, -0.5f),
			vec3(0.5f,  0.5f, -0.5f),

			vec3(-0.5f,  0.5f,  0.5f),
			vec3(-0.5f,  0.5f, -0.5f),
			vec3(0.5f,  0.5f, -0.5f),
			vec3(-0.5f,  0.5f,  0.5f),
			vec3(0.5f,  0.5f, -0.5f),
			vec3(0.5f,  0.5f,  0.5f),

			vec3(-0.5f, -0.5f,  0.5f),
			vec3(-0.5f, -0.5f, -0.5f),
			vec3(0.5f, -0.5f, -0.5f),
			vec3(-0.5f, -0.5f,  0.5f),
			vec3(0.5f, -0.5f, -0.5f),
			vec3(0.5f, -0.5f,  0.5f),

			vec3(-0.5f,  0.5f, -0.5f),
			vec3(-0.5f, -0.5f, -0.5f),
			vec3(-0.5f, -0.5f,  0.5f),
			vec3(-0.5f,  0.5f, -0.5f),
			vec3(-0.5f, -0.5f,  0.5f),
			vec3(-0.5f,  0.5f,  0.5f),

			vec3(0.5f,  0.5f, -0.5f),
			vec3(0.5f, -0.5f, -0.5f),
			vec3(0.5f, -0.5f,  0.5f),
			vec3(0.5f,  0.5f, -0.5f),
			vec3(0.5f, -0.5f,  0.5f),
			vec3(0.5f,  0.5f,  0.5f)
		};

		Mesh* mesh = new Mesh();
		mesh->setPositions(vertices, sizeof(vertices) / sizeof(vec3));

		return mesh;
	}

	Mesh* cubeIndexed()
	{
		vec3 vertex_positions[] =
		{
			vec3(-0.5f, -0.5f, 0.5f),
			vec3(0.5f, -0.5f, 0.5f),
			vec3(0.5f, 0.5f,  0.5f),
			vec3(-0.5f, 0.5f, 0.5f),

			vec3(-0.5f, -0.5f, -0.5f),
			vec3(0.5f, -0.5f, -0.5f),
			vec3(0.5f, 0.5f, -0.5f),
			vec3(-0.5f, 0.5f, -0.5f),

			vec3(0.5f, -0.5f, 0.5f),
			vec3(0.5f, -0.5f, -0.5f),
			vec3(0.5f, 0.5f, -0.5f),
			vec3(0.5f, 0.5f, 0.5f),

			vec3(-0.5f, -0.5f, 0.5f),
			vec3(-0.5f, -0.5f, -0.5f),
			vec3(-0.5f, 0.5f, -0.5f),
			vec3(-0.5f, 0.5f, 0.5f),

			vec3(-0.5f, 0.5f, 0.5f),
			vec3(0.5f, 0.5f, 0.5f),
			vec3(0.5f, 0.5f, -0.5f),
			vec3(-0.5f, 0.5f, -0.5f),

			vec3(-0.5f, -0.5f, 0.5f),
			vec3(0.5f,  -0.5f, 0.5f),
			vec3(0.5f,  -0.5f, -0.5f),
			vec3(-0.5f, -0.5f, -0.5f)
		};

		vec3 vertex_normals[] =
		{
			vec3(0.f, 0.f, 1.f),
			vec3(0.f, 0.f, 1.f),
			vec3(0.f, 0.f, 1.f),
			vec3(0.f, 0.f, 1.f),

			vec3(0.f, 0.f, -1.f),
			vec3(0.f, 0.f, -1.f),
			vec3(0.f, 0.f, -1.f),
			vec3(0.f, 0.f, -1.f),

			vec3(1.f, 0.f, 0.f),
			vec3(1.f, 0.f, 0.f),
			vec3(1.f, 0.f, 0.f),
			vec3(1.f, 0.f, 0.f),

			vec3(-1.f, 0.f, 0.f),
			vec3(-1.f, 0.f, 0.f),
			vec3(-1.f, 0.f, 0.f),
			vec3(-1.f, 0.f, 0.f),

			vec3(0.f, 1.0f, 0.f),
			vec3(0.f, 1.f, 0.f),
			vec3(0.f, 1.f, 0.f),
			vec3(0.f, 1.f, 0.f),

			vec3( 0.f, -1.f, 0.f),
			vec3(0.f, -1.f, 0.f),
			vec3(0.f, -1.f, 0.f),
			vec3(0.f, -1.f, 0.f)
		};

		unsigned int indices[] =
		{
			0, 1, 2,
			2, 3, 0,

			4, 7, 6,
			6, 5, 4,

			8, 9, 10,
			10, 11, 8,

			12, 15, 14,
			14, 13, 12,

			16, 17, 18,
			18, 19, 16,

			20, 23, 22,
			22, 21, 20,
		};

		return NULL;
	}
};