#pragma once

#include <iostream>
#include "Mesh.h"

class Model
{
public:
	Model();
	~Model();

	std::string name;
	Mesh* mesh_array;
	unsigned int mesh_count;

	void createMeshes(int count);
	void print();
	bool hasMeshes();
};