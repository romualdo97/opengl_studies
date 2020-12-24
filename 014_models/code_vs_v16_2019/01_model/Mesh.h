#pragma once
#include <string>
#include <vector>

// shader
#include <RJcustoms/Shader.h>

// for transformations
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

// Vertex attributes that will be passed to the OpenGL Pipeline
// We guarantee that this Struct aligned in memory (no padding is added by compiler)
// since all structs have members of the same type (floats = 4 bytes) targeting a 64-bit (8bytes) processor
// Read more here: https://www.javatpoint.com/structure-padding-in-c#:~:text=Structure%20padding%20is%20a%20concept,align%20the%20data%20in%20memory.
// Or here: https://en.wikipedia.org/wiki/Data_structure_alignment
// Or shearch: what is padding in C struct?
struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture
{
	unsigned int Id;
	std::string Type;
	const char* Path; // we store the path of the texture to compare with other textures
};

class Mesh
{
public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void draw(Shader &shader);

private:
	void setupMesh();

public:
	// mesh data
	std::vector<Vertex>       vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture>      textures;

private:
	// render data
	unsigned int VAO, VBO, EBO;
};

