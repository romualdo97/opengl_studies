#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <iostream>

#include "Shader.h"
#include <stb\stb_image.h>

#define W 960
#define H 480
#define WINDOW_TITLE "Texture sampling exercise"

void resize_framebuffer_cb(GLFWwindow *window, int w, int h);
void process_input(GLFWwindow *window);
void createindow();

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
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
	{
		std::cout << "Could not load GLAD\n";
		return -1;
	}

	// register glfw events
	glfwSetFramebufferSizeCallback(window, resize_framebuffer_cb);

	// ======================================================================
	// create new shader
	Shader myShader("Shaders/myShader");

	// ======================================================================
	// load texture data using stb_image header library
	int i_w, i_h, nrChannels;
	unsigned char *image_data = stbi_load("wall.jpg", &i_w, &i_h, &nrChannels, 0);
	
	// ======================================================================
	// generate opengl texture object
	unsigned int texture;
	glGenTextures(1, &texture);
	// bind for configuration
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	// configure wrap mode in s and t dimensions
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// configure minification and magnification filters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// put imge_data to previously created opengl texture object
	if (image_data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, i_w, i_h, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Error loading texture data\n";
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	// free memory
	stbi_image_free(image_data);

	// ======================================================================
	
	// triangle vertex data 
	float vertex_data[] = {
		// positions (in NDC)				// colors						// texture coordinates
		-0.3f, 0.5f, 0.0f,	/*left-top*/	1.0f, 0.0f, 0.0f,	/*red*/		0.0, 1.0,
		0.3f, 0.5f, 0.0f,	/*right-top*/	0.0f, 1.0f, 0.0f,	/*green*/	1.0, 1.0,
		0.0f, -0.5f, 0.0f,	/*botton*/		0.0f, 0.0f, 1.0f,	/*blue*/	0.5, 0.0
	};

	// generate VAO for store status of subsequent "vertex attribute" calls
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); // bind VAO

	// generate VBO for allocate memory in GPU
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	// bind VBO with GL_ARRAY_BUFFER target
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// copy data from CPU to GPU
	//		use the currently bounded buffer to GL_ARRAY_BUFFER as container
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

	// tell OpenGL how it should interpret the vertex data(per
	// vertex attribute) using glVertexAttribPointer:
	// glVertexAttribPointer(index = [vertex attrib location remember the layout (location = n) keyword in vertex shader], 
	//						size = [is vec2 = 2, vec3 = 3, etc..],
	//						type = [GL_FLOAT, GL_BOOL, etc..], 
	//						normalize = [opengl should normalize the given data?],
	//						stride = [distance between each "position" ternas in VBO],
	//						start = [whare is the start index of "position"?];
	
	// indicate which part of vertex data are vertex positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0); // enable the vertex attribute at location 0

	// indicate which part of vertex data are vertex colors
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1); // enable the vertex attribute at location 1

	// indicate which part of vertex data are texture coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2); // enable the vertex attribute at location 2
	// ======================================================================

	// start viewport
	glViewport(0, 0, W, H);
	
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2, 0.5, 0.2, 1.0); // set the clear color
		glClear(GL_COLOR_BUFFER_BIT); // clear color buffer bitfield

		process_input(window);

		/*
		Note that finding the uniform location does not require 
		you to use the shader program first, but updating a uniform
		does require you to first use the program (by calling
		glUseProgram), because it sets the uniform on the currently
		active shader program.
		*/

		// locate uColor uniform
		float t = (float)glfwGetTime();
		int uTimeLoc = glGetUniformLocation(myShader.ID, "uTime");

		/*Bind the texture before calling the glDrawElements
		and it will then automatically assign the texture
		to the fragment shader's sampler:*/
		glBindTexture(GL_TEXTURE_2D, texture);

		myShader.use();	//glUseProgram(shader_program); // use a shader program
		glUniform1f(uTimeLoc, t);

		glBindVertexArray(VAO); // bind object VAO
		glDrawArrays(GL_TRIANGLES, 0, 3); // draw triangle

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

void create_window()
{

}