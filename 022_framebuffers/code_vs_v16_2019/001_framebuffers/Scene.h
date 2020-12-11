#pragma once

// for transformations
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

// for drawing a box
#include "Box.h"

glm::vec3 cameraPos(7.0f);
glm::vec3 lightPos(1.2f, 1.0f, 0.0f);
bool isProjection = true;
bool isKeyDown = false;

// ======================================================================
// create new shader
Shader* myShader = NULL; // Shader program for object that will be illuminated
Shader* lightShader = NULL; // Shader program for light source

// ======================================================================
// send data to GPU for drawing a box
Box* box = NULL;

// box related
glm::vec3 boxColor(1.0f, 1.0f, 1.0f);

// light related
glm::vec3 lightColor(1.0);


// ======================================================================
// texture objects
unsigned int texture;
unsigned int texture2;

void setupScene()
{
	// ======================================================================
	// create new shader
	myShader = new Shader("Shaders/myShader"); // Shader program for object that will be illuminated
	lightShader = new Shader("Shaders/light"); // Shader program for light source

	// ======================================================================
	// send data to GPU for drawing a box
	box = new Box();

	// ======================================================================
	// Load diffuse map and specular map textures

	// tell stb library to flip images in load
	stbi_set_flip_vertically_on_load(true);

	// load texture data using stb_image header library
	int i_w, i_h, nrChannels;
	unsigned char* image_data = stbi_load("container2.png", &i_w, &i_h, &nrChannels, 0);

	// load awesomeface image data
	int i2_w, i2_h, nrChannels2;
	unsigned char* image_data2 = stbi_load("container2_specular.png", &i2_w, &i2_h, &nrChannels2, 0);

	// ======================================================================
	// generate first texture
	glGenTextures(1, &texture);

	// bind for configuration
	glActiveTexture(GL_TEXTURE0); // activate texture unit 0
	glBindTexture(GL_TEXTURE_2D, texture);

	// put imge_data to previously created opengl texture object
	if (image_data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, i_w, i_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Error loading texture data\n";
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(image_data); // free memory

	// generate second texture
	glGenTextures(1, &texture2);
	// activate texture unit 1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	if (image_data2)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, i2_w, i2_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Error loading texture data\n";
	}

	stbi_image_free(image_data2);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void drawScene()
{
	/*
	Note that finding the uniform location does not require
	you to use the shader program first, but updating a uniform
	does require you to first use the program (by calling
	glUseProgram), because it sets the uniform on the currently
	active shader program.
	*/

	// get location of objectColor and lightColor
	int uObjectColorLoc = glGetUniformLocation(myShader->ID, "uObjectColor");
	int uLightColorLoc = glGetUniformLocation(myShader->ID, "uLightColor");
	int uLightPosLoc = glGetUniformLocation(myShader->ID, "uLightPos");
	int uViewPosLoc = glGetUniformLocation(myShader->ID, "uViewPos");

	// get location of diffuse and specular maps
	int uDiffuseMapLoc = glGetUniformLocation(myShader->ID, "uDiffuseMap");
	int uSpecularMapLoc = glGetUniformLocation(myShader->ID, "uSpecularMap");

	// get uniform uModel, uView and uProj location at shader program
	int uModelLoc = glGetUniformLocation(myShader->ID, "uModel");
	int uViewLoc = glGetUniformLocation(myShader->ID, "uView");
	int uProjLoc = glGetUniformLocation(myShader->ID, "uProj");

	// get location of objectColor and lightColor
	int uLightColorSourceLoc = glGetUniformLocation(lightShader->ID, "uLightColor");

	// get uniform uModel, uView and uProj location at shader program
	int uModelSourceLoc = glGetUniformLocation(lightShader->ID, "uModel");
	int uViewSourceLoc = glGetUniformLocation(lightShader->ID, "uView");
	int uProjSourceLoc = glGetUniformLocation(lightShader->ID, "uProj");


	// Just for animating
	float t = (float)glfwGetTime();
	float s = 0.5 + 0.5 * sin((float)glfwGetTime());

	// ======================================================================
	// Draw box that will be illuminated

	// slect shader program and pass uniforms
	myShader->use(); //glUseProgram(shader_program); // use a shader program
	glBindVertexArray(box->VAO); // bind box VAO

	// bind opengl object "texture" to GL_TEXTURE_2D target
	// in texture unit 0 (GL_TEXTURE0)
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	// bind opengl object "texture2" to GL_TEXTURE_2D target
	// in texture unit 1 (GL_TEXTURE0)
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	// specify what texture unit should use the uniform GLSL sampler uDiffuseMap
	glUniform1i(uDiffuseMapLoc, 0); // use texture unit 0

	// specify what texture unit should use the uniform GLSL sampler uSpecularMapLoc
	glUniform1i(uSpecularMapLoc, 1); // use texture unit 1

	// create view matrix
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(cameraPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Generates a Look At matrix // note that we're translating the scene in the reverse direction of where we want to move
	glUniformMatrix4fv(uViewLoc, 1, GL_FALSE, glm::value_ptr(view));

	// create projection matrix
	glm::mat4 projection;
	if (isProjection)
	{
		projection = glm::perspective(glm::radians(45.0f), (float)(W / H), 0.1f, 100.0f);
	}
	else
	{
		projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);
	}

	glUniformMatrix4fv(uProjLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// pass objectColor, lightColor, lightPos to illuminated object
	glUniform3fv(uObjectColorLoc, 1, glm::value_ptr(boxColor));
	glUniform3fv(uLightColorLoc, 1, glm::value_ptr(lightColor));
	glUniform3fv(uLightPosLoc, 1, glm::value_ptr(lightPos));
	glUniform3fv(uViewPosLoc, 1, glm::value_ptr(cameraPos));

	// Draw a box
	box->model = glm::mat4(1.0f); //glm::mat4 model;
	box->setRotationY(t); //model = glm::rotate(model, t, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uModelLoc, 1, GL_FALSE, glm::value_ptr(box->model)); // GLint location,	GLsizei count,	GLboolean transpose, const GLfloat* value
	box->draw();

	// ======================================================================
	// Draw box for the light source

	// slect shader program and pass uniforms
	lightShader->use();	//glUseProgram(shader_program); // use a shader program
	// glBindVertexArray(box.VAO); // bind box VAO // let's use same box VAO

	glUniformMatrix4fv(uViewSourceLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(uProjSourceLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// pass lightColor to light object
	glUniform3fv(uLightColorSourceLoc, 1, glm::value_ptr(lightColor));

	// draw another box for the light source
	box->model = glm::mat4(1.0f); //glm::mat4 model;
	box->setPosition(lightPos); //model = glm::rotate(model, t, glm::vec3(0.0f, 1.0f, 0.0f));
	box->model = glm::scale(box->model, glm::vec3(0.2f)); // Scale down the light source model
	glUniformMatrix4fv(uModelLoc, 1, GL_FALSE, glm::value_ptr(box->model)); // GLint location,	GLsizei count,	GLboolean transpose, const GLfloat* value
	box->draw();
}