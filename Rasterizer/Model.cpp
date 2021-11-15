#include "Model.h"

Model::Model()
{
	name = "";
	mesh_array = NULL;
	mesh_count = 0;
}

Model::~Model()
{
	if (mesh_array != NULL)
	{
		delete[] mesh_array;
	}
}

void Model::createMeshes(const int count)
{
	if (count <= 0)
	{
		return;
	}

	if (mesh_array != NULL)
	{
		delete[] mesh_array;
	}

	mesh_array = new Mesh[count];
	mesh_count = count;
}

bool Model::hasMeshes()
{
	return mesh_array != NULL && mesh_count > 0;
}

void Model::print()
{
	printf("model %s has %u meshes\n", name.c_str(), mesh_count);

	if (hasMeshes())
	{
		for (int i = 0; i < (int)mesh_count; ++i)
		{
			mesh_array[i].print();
		}
	}
}