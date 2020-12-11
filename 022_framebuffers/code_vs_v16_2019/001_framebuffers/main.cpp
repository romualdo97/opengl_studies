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

#define W 820
#define H 820
#define WINDOW_TITLE "Animating mesh using matrix transformations"

// The scene code
#include "Scene.h"
#include "Quad.h"

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
	// Prepare framebuffer

	// Create a Frame Buffer Object (FBO)
	unsigned int frameBuffer = 0;
	glGenFramebuffers(1, &frameBuffer);

	// bind the object to the GL_FRAMEBUFFER target so we can configure it
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer); // Or you can use GL_READ_FRAMEBUFFER or GL_DRAW_FRAMEBUFFER 

	// Unfortunately, we can't use our framebuffer yet because it is not complete.
	// For a framebuffer to be complete the following requirements have to be satisfied:

	// - We have to attach at least one buffer (color, depth or stencil buffer).
	// - There should be at least one color attachment.
	// - All attachments should be complete as well (reserved memory).
	// - Each buffer should have the same number of samples.

	// Next we create a texture image that we attach as a color attachment to the framebuffer.
	// We set the texture's dimensions equal to the width and height of the window and keep its data uninitialized:

	unsigned int texColorBuffer;
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, W, H, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

	// We also want to make sure OpenGL is able to do depth testing(and optionally stencil testing)
	// so we have to make sure to add a depth(and stencil) attachment to the framebuffer.

	// Since we'll only be sampling the color buffer and not the other buffers we can create a
	// renderbuffer object for this purpose.

	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, W, H);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// we attach the renderbuffer object to the depth and stencil attachment of the framebuffer:

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	// Then we want to check if the framebuffer is complete and if it's not, we print an error message.
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	// Be sure to unbind the framebuffer to make sure we're not accidentally rendering to the wrong framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// ======================================================================
	// Setup scene

	// Setup the main scene
	setupScene();

	// Quad for drawing the scene to
	Quad quad;

	// Shader for drawing the scene into quad
	Shader secondPassShader("Shaders/secondPass");

	// ======================================================================
	// update and draw commands

	// start viewport
	glViewport(0, 0, W, H);
	
	while (!glfwWindowShouldClose(window))
	{
		process_input(window);

		// So, to draw the scene to a single texture we'll have to take the following steps:
		// 1. Render the scene as usual with the new framebuffer bound as the active framebuffer.
		// 2. Bind to the default framebuffer.
		// 3. Draw a quad that spans the entire screen with the new framebuffer's color buffer as its texture.

		// First pass
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glClearColor(0.2, 0.5, 0.2, 1.0); // set the clear color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
		drawScene();

		// Second pass
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		secondPassShader.use();
		glDisable(GL_DEPTH_TEST);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texColorBuffer);
		quad.draw();

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

	// ======================================================
	// Camera rotating around origin
	const float cameraSpeed = 0.001f; // adjust accordingly
	static float cameraPosT = 0.0f;
	static const float PI = glm::pi<float>();
	const float radius = 3.0f;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPosT -= cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPosT += cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos.y += cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos.y -= cameraSpeed;


	cameraPos.x = cos(PI * cameraPosT) * radius;
	cameraPos.z = sin(PI * cameraPosT) * radius;

	// ======================================================
	// Move the light
	const float lightSpeed = 0.001f;
	if (glfwGetKey(window, GLFW_KEY_UP))
		lightPos.z += lightSpeed;
	if (glfwGetKey(window, GLFW_KEY_DOWN))
		lightPos.z -= lightSpeed;
	if (glfwGetKey(window, GLFW_KEY_LEFT))
		lightPos.x -= lightSpeed;
	if (glfwGetKey(window, GLFW_KEY_RIGHT))
		lightPos.x += lightSpeed;

	// ======================================================
	// Change projection mode
	if (glfwGetKey(window, GLFW_KEY_SPACE) && !isKeyDown)
	{
		isProjection = !isProjection;
		isKeyDown = true;
	}

	if (!glfwGetKey(window, GLFW_KEY_SPACE) && isKeyDown)
	{
		//isProjection = !isProjection;
		isKeyDown = false;
	}
}