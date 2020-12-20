#pragma once
#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// for transformations
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

class Shader
{
public:
	// Constructor: Specify vertex and fragment shader path
	// Shader("MyFrag.frag", "myVert.vert");
	Shader(char const *vertexPath, char const *fragmentPath);
	// Constructor: Specify just a shader name (no shader file extension)
	// this constructor assumes the vertex and fragment shader are named equally
	// with its respective different file extensions, e.g: Shader("MyShader"); assumes
	// two files; MyShader.frag, MyShader.vert
	Shader(GLchar const *shaderName);
	// use shader program
	void use();
	// utility uniform functions
	void setMatrix(std::string const& name, glm::mat4x4& value);
	void setBool(std::string const &name, bool value);
	void setInt(std::string const &name, int value);
	void setFloat(std::string const &name, float value);
	void setFloatArray(std::string const& name, GLsizei arraySize, float* firstItem);
	void setVec3(std::string const& name, glm::vec3& value);
	void setVec3(std::string const& name, float x, float y, float z);
	void setVec3Array(std::string const& name, GLsizei arraySize, glm::vec3 firstItem);

	~Shader();
	unsigned int ID;
private:
	void compileShader(char const *vertex_src, char const *fragment_src);
	void checkCompileError(unsigned int shader, unsigned int stage, unsigned int const status);
};

