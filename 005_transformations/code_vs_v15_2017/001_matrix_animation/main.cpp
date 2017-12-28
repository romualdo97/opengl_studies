// for opengl context and its commands
#include <glad\glad.h>
#include <GLFW\glfw3.h>

// cpp libraries
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
	// tell stb library to flip images in load
	stbi_set_flip_vertically_on_load(true);
	
	// load texture data using stb_image header library
	int i_w, i_h, nrChannels;
	unsigned char *image_data = stbi_load("wall.jpg", &i_w, &i_h, &nrChannels, 0);

	// load awesomeface image data
	int i2_w, i2_h, nrChannels2;
	unsigned char *image_data2 = stbi_load("awesomeface.png", &i2_w, &i2_h, &nrChannels2, 0);

	
	// ======================================================================
	// generate first texture
	unsigned int texture;
	glGenTextures(1, &texture);
	// bind for configuration
	glActiveTexture(GL_TEXTURE0); // activate texture unit 0
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
	stbi_image_free(image_data); // free memory

	// generate second texture
	unsigned int texture2;
	glGenTextures(1, &texture2);
	// activate texture unit 1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	if (image_data2)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, i2_w, i2_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Error loading texture data\n";

	}

	stbi_image_free(image_data2);
	glBindTexture(GL_TEXTURE_2D, 0);

	// ======================================================================
	
	// triangle vertex data 
	float vertex_data[] = {
		// positions (in NDC)				// colors						// texture coordinates
		-0.5f, 0.5f, 0.0f,	/*left-top*/	1.0f, 0.0f, 0.0f,	/*red*/		0.0, 1.0,
		0.5f, 0.5f, 0.0f,	/*right-top*/	0.0f, 1.0f, 0.0f,	/*green*/	1.0, 1.0,
		-0.5f, -0.5f, 0.0f,	/*left-bttm*/	0.0f, 0.0f, 1.0f,	/*blue*/	0.0, 0.0,
		0.5f, -0.5f, 0.0f,	/*right-bttm*/  0.0f, 1.0f, 1.0f,	/*#0ff*/	1.0, 0.0
	};

	// index drawing data for draw a quad
	unsigned int index_drawing_data[] = {
		0, 1, 2, /*first triangle*/
		1, 3, 2	 /*second triangle*/
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
	
	// generate EBO
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	// bind EBO to GL_ELEMENT_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// pass EBO data from CPU to GPU
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_drawing_data), index_drawing_data, GL_STATIC_DRAW);
	
	// ======================================================================
	// update and draw commands

	// start viewport
	glViewport(0, 0, W, H);

	/*bind opengl object "texture" to GL_TEXTURE_2D target
	in texture unit 0 (GL_TEXTURE0)*/
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	/*bind opengl object "texture2" to GL_TEXTURE_2D target
	in texture unit 1 (GL_TEXTURE0)*/
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	/*
	Note that finding the uniform location does not require
	you to use the shader program first, but updating a uniform
	does require you to first use the program (by calling
	glUseProgram), because it sets the uniform on the currently
	active shader program.
	*/
	
	// locate uTime uniform
	int uTimeLoc = glGetUniformLocation(myShader.ID, "uTime");
	
	// use shader program and pass uniforms
	myShader.use();	//glUseProgram(shader_program); // use a shader program

	// specify what texture unit should use the uniform GLSL sampler uTextureA
	int uTexALoc = glGetUniformLocation(myShader.ID, "uTextureA");
	glUniform1i(uTexALoc, 0); // use texture unit 0

	// specify what texture unit should use the uniform GLSL sampler uTextureB
	int uTexBLoc = glGetUniformLocation(myShader.ID, "uTextureB");
	glUniform1i(uTexBLoc, 1); // use texture unit 1

	// pass myTransformMatrix to uTransform uniform in shader program
	int uTransLoc = glGetUniformLocation(myShader.ID, "uTransform");
	
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2, 0.5, 0.2, 1.0); // set the clear color
		glClear(GL_COLOR_BUFFER_BIT); // clear color buffer bitfield

		process_input(window);

		float t = (float)glfwGetTime();
		float s = 0.5 + 0.5 * sin((float)glfwGetTime());

		glUniform1f(uTimeLoc, t);

		// rotate 45deg and scale 0.5
		glm::mat4 myTransformMatrix;
		myTransformMatrix = glm::rotate(myTransformMatrix, t, glm::vec3(0.0, 0.0, 1.0));
		myTransformMatrix = glm::scale(myTransformMatrix, glm::vec3(s, s, s));
		glUniformMatrix4fv(uTransLoc, 1, GL_FALSE, glm::value_ptr(myTransformMatrix));

		glBindVertexArray(VAO); // bind object VAO
		//glDrawArrays(GL_TRIANGLES, 0, 3); // draw triangle
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // draw a quad

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