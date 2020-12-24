#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) :
	vertices(vertices), indices(indices), textures(textures)
{
	setupMesh();
}

void Mesh::draw(Shader shader)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	for (unsigned int i = 0; i < textures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding

		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = textures[i].Type;

		if (name == "textureDiffuse")
		{
			number = std::to_string(diffuseNr++);
		}
		else if (name == "textureSpecular")
		{
			number = std::to_string(diffuseNr++);
		}

		// Convention will be
		// uMaterial.textureDiffuse1
		// uMaterial.textureDiffuse2
		// uMaterial.textureDiffuse3
		// uMaterial.textureSpecular1
		// uMaterial.textureSpecular2
		shader.setInt(("uMaterial." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].Id);
	}

	// Reset to texture unit 0 
	glActiveTexture(GL_TEXTURE0);

	// Draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0); // Unbind VAO
}

void Mesh::setupMesh()
{
	// Show size of structs
	std::cout << "sizeof(glm::vec3): " << sizeof(glm::vec3) << std::endl; // sizeof 12 bytes
	std::cout << "sizeof(Vertex): " << sizeof(Vertex) << std::endl; // sizeof 12 * 2 + 8 = 32 bytes

	// Generate buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Save subsequent state calls
	glBindVertexArray(VAO); // bind VAO

	// Set data to VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW); // Copy data from CPU to GPU

	// Set data to EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // bind EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Vertex) * indices.size(), &indices[0], GL_STATIC_DRAW); // copy data from CPU to GPU

	// Tell OpenGL how to understand the sended VBO data
	
	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	// Total sended data
	std::cout << "sizeof(Vertex) * vertices.size() : " << sizeof(Vertex) * vertices.size() << std::endl;
	std::cout << "sizeof(unsigned int) * indices.size() : " << sizeof(unsigned int) * indices.size() << std::endl;
}

