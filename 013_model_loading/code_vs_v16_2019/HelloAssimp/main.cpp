#include <iostream>
#include <assimp/Importer.hpp> // C++ importer interface
#include <assimp/scene.h> // Output data structure
#include <assimp/postprocess.h> // Post processing flags

void DoTheSceneProcessing(const aiScene* scene)
{
	std::cout << "Assimp scene loaded succesfully\n";
	std::cout << "Has camera: " << scene->HasCameras() << std::endl;
	std::cout << "Has meshes: " << scene->HasMeshes() << std::endl;
	
	std::cout << "\nMATERIALS\n";
	std::cout << "Num materials: " << scene->mNumMaterials << std::endl;
	for (int i = 0; i < scene->mNumMaterials; ++i)
	{
		std::cout << "mMaterials[" << i << "].GetName() : " << scene->mMaterials[i]->GetName().C_Str() << std::endl;
	}

	std::cout << "\nMESHES\n";
	std::cout << "Num meshes: " << scene->mNumMeshes << std::endl;
	for (int i = 0; i < scene->mNumMeshes; ++i)
	{
		std::cout << "mMeshes[" << i << "].mName : " << scene->mMeshes[i]->mName.C_Str() << std::endl;
	}
}

bool DoTheImportThing(const std::string& filePath)
{
	// Create an instance of the Importer class
	Assimp::Importer importer;

	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll
	// probably to request more postprocessing than we do in this example.

	const aiScene* scene = importer.ReadFile(filePath,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	// If the import failed, report it
	if (!scene) {
		std::cout << importer.GetErrorString() << std::endl;
		return false;
	}

	// Now we can access the file's contents.
	DoTheSceneProcessing(scene);

	// We're done. Everything will be cleaned up by the importer destructor
	return false;
}

int main()
{
	std::cout << "Hello Assimp!!!\n";
	DoTheImportThing("Car.obj");
	return 0;
}