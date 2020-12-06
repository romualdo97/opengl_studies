// use winding order to draw correctly cube faces? else use depth testing method
// #define USE_CUBE_CULLING

// for OpenGL context and its commands
#include <glad\glad.h>
#include <GLFW\glfw3.h>

// Cpp libraries
#include <iostream>

// for data loading and shader compiling
#include <RJcustoms\Shader.h>
#include <stb\stb_image.h>

// for transformations
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

// for drawing a box
#include "Box.h"

#define W 820
#define H 820
#define WINDOW_TITLE "Animating mesh using matrix transformations"

void resize_framebuffer_cb(GLFWwindow *window, int w, int h);
void process_input(GLFWwindow *window);

int main(void)
{
	// configure window and context
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window
	GLFWwindow *window = glfwCreateWindow(W, H, WINDOW_TITLE, NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Window could not be created\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// init glad for call opengl functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Could not load GLAD\n";
		return -1;
	}

	// register glfw events
	glfwSetFramebufferSizeCallback(window, resize_framebuffer_cb);

	// ======================================================================
	// create new shader
	Shader myShader("Shaders/myShader"); // Shader program for object that will be illuminated
	Shader lightShader("Shaders/light"); // Shader program for light source

	// ======================================================================
	// send data to GPU for drawing a box
	Box box;

	// box related
	glm::vec3 boxColor(1.0f, 0.5f, 0.31f);

	// light related
	glm::vec3 lightColor(1.0);
	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);


	// ======================================================================
	// update and draw commands

	// start viewport
	glViewport(0, 0, W, H);

	/*
	Note that finding the uniform location does not require
	you to use the shader program first, but updating a uniform
	does require you to first use the program (by calling
	glUseProgram), because it sets the uniform on the currently
	active shader program.
	*/

	// get location of objectColor and lightColor
	int uObjectColorLoc = glGetUniformLocation(myShader.ID, "uObjectColor");
	int uLightColorLoc = glGetUniformLocation(myShader.ID, "uLightColor");

	// get uniform uModel, uView and uProj location at shader program
	int uModelLoc = glGetUniformLocation(myShader.ID, "uModel");
	int uViewLoc = glGetUniformLocation(myShader.ID, "uView");
	int uProjLoc = glGetUniformLocation(myShader.ID, "uProj");

	// get location of objectColor and lightColor
	int uLightColorSourceLoc = glGetUniformLocation(lightShader.ID, "uLightColor");

	// get uniform uModel, uView and uProj location at shader program
	int uModelSourceLoc = glGetUniformLocation(lightShader.ID, "uModel");
	int uViewSourceLoc = glGetUniformLocation(lightShader.ID, "uView");
	int uProjSourceLoc = glGetUniformLocation(lightShader.ID, "uProj");
	
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2, 0.5, 0.2, 1.0); // set the clear color
#ifdef USE_CUBE_CULLING
		glClear(GL_COLOR_BUFFER_BIT);
#else
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear color buffer bitfield
#endif // usecub

		process_input(window);

		// Just for animating
		float t = (float)glfwGetTime();
		float s = 0.5 + 0.5 * sin((float)glfwGetTime());

		// ======================================================================
		// Draw box that will be illuminated

		// slect shader program and pass uniforms
		myShader.use();	//glUseProgram(shader_program); // use a shader program
		glBindVertexArray(box.VAO); // bind box VAO

		// create view matrix
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -7.0f)); // note that we're translating the scene in the reverse direction of where we want to move
		glUniformMatrix4fv(uViewLoc, 1, GL_FALSE, glm::value_ptr(view));

		// creat projection matrix
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), (float)(W / H), 0.1f, 100.0f);
		glUniformMatrix4fv(uProjLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// pass objectColor and lightColor to illuminated object
		glUniform3fv(uObjectColorLoc, 1, glm::value_ptr(boxColor));
		glUniform3fv(uLightColorLoc, 1, glm::value_ptr(lightColor));

		// Draw a box
		box.model = glm::mat4(1.0f); //glm::mat4 model;
		box.setRotationY(t); //model = glm::rotate(model, t, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uModelLoc, 1, GL_FALSE, glm::value_ptr(box.model)); // GLint location,	GLsizei count,	GLboolean transpose, const GLfloat* value
		box.draw();

		// ======================================================================
		// Draw box for the light source

		// slect shader program and pass uniforms
		lightShader.use();	//glUseProgram(shader_program); // use a shader program
		// glBindVertexArray(box.VAO); // bind box VAO // let's use same box VAO

		glUniformMatrix4fv(uViewSourceLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(uProjSourceLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// pass lightColor to light object
		glUniform3fv(uLightColorSourceLoc, 1, glm::value_ptr(lightColor));

		// draw another box for the light source
		box.model = glm::mat4(1.0f); //glm::mat4 model;
		box.setPosition(lightPos); //model = glm::rotate(model, t, glm::vec3(0.0f, 1.0f, 0.0f));
		box.model = glm::scale(box.model, glm::vec3(0.2f)); // Scale down the light source model
		glUniformMatrix4fv(uModelLoc, 1, GL_FALSE, glm::value_ptr(box.model)); // GLint location,	GLsizei count,	GLboolean transpose, const GLfloat* value
		box.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}

void resize_framebuffer_cb(GLFWwindow *window, int w, int h)
{
	glViewport(0, 0, w, h);
}

void process_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(window, true);
	}
}