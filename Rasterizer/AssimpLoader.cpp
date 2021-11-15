#include "AssimpLoader.h"
#include "Matrix.h"
#include <map>
#include "Texture.h"

extern std::map<std::string, Texture> textureCache;

AssimpLoader::AssimpLoader(void)
{
}

AssimpLoader::~AssimpLoader(void)
{
}

bool AssimpLoader::importFile(const char *folderpath, const char *filename, unsigned int flags, Model *model)
{
	std::string path = folderpath;
	path += filename;

	Assimp::Importer importer;
	const aiScene* aiModel = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenUVCoords | flags |  aiProcess_FlipWindingOrder);
	if (aiModel == NULL)
	{
		 const char* error = importer.GetErrorString();
		 std::cout << error << std::endl;
		 std::cout << "error: failed to import " << path << std::endl;
		 return false;
	}
	
	if (aiModel->HasMeshes())
	{
		model->createMeshes(aiModel->mNumMeshes);
		model->name = aiModel->mRootNode->mName.C_Str();

		for (int i = 0; i < aiModel->mNumMeshes; ++i)
		{
			aiMesh* aiMesh = aiModel->mMeshes[i];
			aiString textureFileName;
			aiModel->mMaterials[aiMesh->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &textureFileName);
			if (strcmp(textureFileName.C_Str(), "") != 0)
			{
				model->mesh_array[i].textureKey = textureFileName.C_Str();
			}

			model->mesh_array[i].name = aiMesh->mName.C_Str();

			if(aiMesh->HasPositions())
			{
				model->mesh_array[i].setPositions((vec3*) aiMesh->mVertices, aiMesh->mNumVertices);
			}
			if (aiMesh->HasNormals())
			{
				model->mesh_array[i].setNormals((vec3*) aiMesh->mNormals, aiMesh->mNumVertices);
			}
			if (aiMesh->HasTextureCoords(0))
			{
				model->mesh_array[i].setUVs((vec3*) aiMesh->mTextureCoords[0], aiMesh->mNumVertices);
			}
		}
	}

	if(aiModel->HasMaterials())
	{
		for (int i = 0; i < aiModel->mNumMaterials; ++i)
		{
			aiString textureFileName;
			aiModel->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &textureFileName);
			if (strcmp(textureFileName.C_Str(), "") != 0)
			{
				std::string textureFullPath = folderpath;
				textureFullPath += textureFileName.C_Str();
				textureCache[textureFileName.C_Str()].load(textureFullPath.c_str());
			}
		}
	}
	
	return true;
}