#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Model.h"

class AssimpLoader
{
public:
	AssimpLoader(void);
	~AssimpLoader(void);
	
	bool importFile(const char *folderpath, const char *filename, unsigned int flags, Model *model);
};