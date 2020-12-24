#pragma once
#include <string>
#include <vector>
#include <RJcustoms/Shader.h>

#include <assimp/Importer.hpp> // C++ importer interface
#include <assimp/scene.h> // Output data structure
#include <assimp/postprocess.h> // Post processing flags

#include "Mesh.h"
#include "TextureUtils.h";

class Model
{
public:
	Model(const char* path);
	void draw(Shader &shader);

private:
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	// Model data
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> texturesLoaded;
};
