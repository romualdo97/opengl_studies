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

#include "Model.h"

#define W 820
#define H 820
#define WINDOW_TITLE "Loading usin custom Model class"

// Draw car or catapult? (select only once at time)
#define DRAW_CAR
//#define DRAW_CATAPULT

#if defined(DRAW_CAR) && defined(DRAW_CATAPULT)
#undef DRAW_CAR // If car and catapult, then draw only catapult
#elif !defined(DRAW_CAR) && !defined(DRAW_CATAPULT)
#define DRAW_CAR // By default draw catapult
#endif //  DRAW_CAR && DRAW_CATAPULT


void resize_framebuffer_cb(GLFWwindow* window, int w, int h);
void process_input(GLFWwindow* window);

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); // by convention (in OpenGL) the camera points towards the negative z-axis
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float distance = 3.0f;

int main(void)
{
	// configure window and context
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window
	GLFWwindow* window = glfwCreateWindow(W, H, WINDOW_TITLE, NULL, NULL);
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
	Shader surfaceShader("Shaders/surface.vert", "Shaders/surfaceUnlit.frag");

	// ======================================================================
	// load models data and send to gpu
	
#ifdef DRAW_CAR
	Model carModel("Models/car.obj");
#endif

#ifdef DRAW_CATAPULT
	Model catapultModel("Models/catapult.obj");
#endif

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

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2, 0.5, 0.2, 1.0); // set the clear color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear color buffer bitfield
		
		process_input(window);

		float t = 0.0f; // (float)glfwGetTime();
		float s = 0.5 + 0.5 * sin((float)glfwGetTime());

		// use shader program and pass uniforms
		surfaceShader.use();

		// pass a global time
		surfaceShader.setFloat("uTime", t);

		// create and pass model matrix
		glm::mat4 model;
		model = glm::rotate(model, t, glm::vec3(1.0f, 1.0f, 0.0f));

#ifdef DRAW_CATAPULT
		model = glm::scale(model, glm::vec3(0.01f)); // Scale down the catapult, its model is to big
#endif

		surfaceShader.setMatrix("uModel", model);
		
		// create and pass view matrix
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp); // Generates a Look At matrix
		surfaceShader.setMatrix("uView", view);

		// create and pass projection matrix
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), (float)(W / H), 0.1f, 1000.0f); // projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);
		surfaceShader.setMatrix("uProj", projection);

		// Draw a car object
		glEnable(GL_DEPTH_TEST);

#ifdef DRAW_CAR
		carModel.draw(surfaceShader);
#endif

#ifdef DRAW_CATAPULT
		catapultModel.draw(surfaceShader);
#endif

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}

void resize_framebuffer_cb(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}

void process_input(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(window, true);
	}

	//const float cameraSpeed = 0.01f; // adjust accordingly
	// Camera moving on plane XY plane
	/*if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;*/

		// Camera rotating around origin
	const float cameraSpeed = 0.001f; // adjust accordingly
	static float cameraPosT = 0.0f;
	static const float PI = glm::pi<float>();
	const float radius = distance;
	cameraPos.x = cos(PI * cameraPosT) * radius;
	cameraPos.z = sin(PI * cameraPosT) * radius;

	glm::vec3 lookAt(0.0f);
	cameraFront = glm::normalize(lookAt - cameraPos);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPosT -= cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPosT += cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		distance -= cameraSpeed * 10.0f;
		if (distance < 1.0f)
		{
			distance = 1.0f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		distance += cameraSpeed * 10.0f;
	}
}