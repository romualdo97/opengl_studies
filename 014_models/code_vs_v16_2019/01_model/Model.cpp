#include "Model.h"

Model::Model(const char* path)
{
    loadModel(path);
}

void Model::draw(Shader& shader)
{
    for (size_t i = 0; i < meshes.size(); ++i)
    {
        meshes[i].draw(shader);
    }
}

void Model::loadModel(std::string path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    directory = path.substr(0, path.find_last_of("/"));
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; ++i)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // Process vertex attributes data
    for (size_t i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex newVertex;

        // Get vertex position
        aiVector3D vertexPosition = mesh->mVertices[i];
        newVertex.Position = glm::vec3(vertexPosition.x, vertexPosition.y, vertexPosition.z);

        // Get vertex normal
        aiVector3D vertexNormal = mesh->mNormals[i];
        newVertex.Normal = glm::vec3(vertexNormal.x, vertexNormal.y, vertexNormal.z);

        // Get vertext texture coordinate
        unsigned int textureCoordChannel = 0;
        if (mesh->HasTextureCoords(textureCoordChannel))
        {
            aiVector3D textureCoord = mesh->mTextureCoords[textureCoordChannel][i];
            newVertex.TexCoords = glm::vec2(textureCoord.x, textureCoord.y);
        }
        else
        {
            newVertex.TexCoords = glm::vec2(0.0f);
        }

        vertices.push_back(newVertex);
    }

    // Process indices
    for (size_t i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        for (size_t j = 0; j < face.mNumIndices; ++j)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    // Process materials
    if (mesh->mMaterialIndex >= 0)
    {
        // A mesh uses only a single material
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        // Load material diffuse textures
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "textureDiffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        
        // Load material specular textures
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "textureSpecular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    // Create new mesh
    Mesh newMesh(vertices, indices, textures);
    return newMesh;
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;

    for (size_t i = 0; i < mat->GetTextureCount(type); ++i)
    {
        // Get texture path
        aiString path;
        mat->GetTexture(type, i, &path);
        const char* cStringPath = path.C_Str();

        // Check if texture was already loaded
        bool loaded = false;
        for (auto texture : texturesLoaded)
        {
            if (strcmp(texture.Path, cStringPath))
            {
                loaded = true;
                break;
            }
        }

        // Don't load again
        if (loaded) break;

        // Create a path relative to the 3D model file
        std::string fullPath = directory + "/" + cStringPath;

        // New texture
        Texture texture;
        texture.Id = load_texture(fullPath.c_str());
        texture.Type = typeName;
        texture.Path = cStringPath;

        // Add to list
        textures.push_back(texture);
        texturesLoaded.push_back(texture);
    }

    return textures;
}
