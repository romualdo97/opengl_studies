#pragma once
#include <glad/glad.h> // // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	// Constructor reads and builds the shader
	Shader(GLchar const *vertexPath, GLchar const *fragmentPath);
	// use shader program
	void use();
	// utility uniform functions
	void setBool(std::string const &name, bool value);
	void setInt(std::string const &name, int value);
	void setFloat(std::string const &name, float value);
	~Shader();
};

