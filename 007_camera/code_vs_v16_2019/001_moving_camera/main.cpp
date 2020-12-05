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

// use winding order to draw correctly cube faces? else use dept testing method
#define USE_CUBE_CULLING

void resize_framebuffer_cb(GLFWwindow *window, int w, int h);
void process_input(GLFWwindow *window);

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); // by convention (in OpenGL) the camera points towards the negative z-axis
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

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
		// FRONT (Z-POSITIVE)
		// positions (in NDC)				// colors						// texture coordinates
		-0.5f, 0.5f, 0.5f,	/*left-top*/	0.0f, 0.0f, 1.0f,	/*red*/		0.0, 1.0,
		0.5f, 0.5f, 0.5f,	/*right-top*/	0.0f, 0.0f, 1.0f,	/*green*/	1.0, 1.0,
		-0.5f, -0.5f, 0.5f,	/*left-bttm*/	0.0f, 0.0f, 1.0f,	/*blue*/	0.0, 0.0,
		0.5f, -0.5f, 0.5f,	/*right-bttm*/  0.0f, 0.0f, 1.0f,	/*#0ff*/	1.0, 0.0,

		// BACK (Z-NEGATIVE)
		// positions (in NDC)				// colors						// texture coordinates
		-0.5f, 0.5f, -0.5f,	/*left-top*/	0.0f, 0.0f, 0.5f,	/*red*/		0.0, 1.0,
		0.5f, 0.5f, -0.5f,	/*right-top*/	0.0f, 0.0f, 0.5f,	/*green*/	1.0, 1.0,
		-0.5f, -0.5f, -0.5f,/*left-bttm*/	0.0f, 0.0f, 0.5f,	/*blue*/	0.0, 0.0,
		0.5f, -0.5f, -0.5f,	/*right-bttm*/  0.0f, 0.0f, 0.5f,	/*#0ff*/	1.0, 0.0,

		// LEFT (X-NEGATIVE)
		// positions (in NDC)				// colors						// texture coordinates
		-0.5f, 0.5f, -0.5f,	/*left-top*/	0.5f, 0.0f, 0.0f,	/*red*/		0.0, 1.0,
		-0.5f, 0.5f, 0.5f,	/*right-top*/	0.5f, 0.0f, 0.0f,	/*green*/	1.0, 1.0,
		-0.5f, -0.5f, -0.5f, /*left-bttm*/	0.5f, 0.0f, 0.0f,	/*blue*/	0.0, 0.0,
		-0.5f, -0.5f, 0.5f, /*right-bttm*/  0.5f, 0.0f, 0.0f,	/*#0ff*/	1.0, 0.0,

		// RIGHT (X-POSITIVE)
		// positions (in NDC)				// colors						// texture coordinates
		0.5f, 0.5f, -0.5f,	/*left-top*/	1.0f, 0.0f, 0.0f,	/*red*/		0.0, 1.0,
		0.5f, 0.5f, 0.5f,	/*right-top*/	1.0f, 0.0f, 0.0f,	/*green*/	1.0, 1.0,
		0.5f, -0.5f, -0.5f, /*left-bttm*/	1.0f, 0.0f, 0.0f,	/*blue*/	0.0, 0.0,
		0.5f, -0.5f, 0.5f, /*right-bttm*/   1.0f, 0.0f, 0.0f,	/*#0ff*/	1.0, 0.0,

		// TOP (Y-POSITIVE)
		// positions (in NDC)				// colors						// texture coordinates
		-0.5f, 0.5f, -0.5f,	/*left-top*/	0.0f, 1.0f, 0.0f,	/*red*/		0.0, 1.0,
		0.5f, 0.5f, -0.5f,	/*right-top*/	0.0f, 1.0f, 0.0f,	/*green*/	1.0, 1.0,
		-0.5f, 0.5f, 0.5f, /*left-bttm*/	0.0f, 1.0f, 0.0f,	/*blue*/	0.0, 0.0,
		0.5f, 0.5f, 0.5f, /*right-bttm*/    0.0f, 1.0f, 0.0f,	/*#0ff*/	1.0, 0.0,

		// BOTTOM (Y-NEGATIVE)
		// positions (in NDC)				// colors						// texture coordinates
		-0.5f, -0.5f, -0.5f,/*left-top*/	0.0f, 0.5f, 0.0F,	/*red*/		0.0, 1.0,
		-0.5f, -0.5f, 0.5f,	/*right-top*/	0.0f, 0.5f, 0.0f,	/*green*/	1.0, 1.0,
		0.5f, -0.5f, -0.5f, /*left-bttm*/	0.0f, 0.5f, 0.0f,	/*blue*/	0.0, 0.0,
		0.5f, -0.5f, 0.5f, /*right-bttm*/   0.0f, 0.5f, 0.0f,	/*#0ff*/	1.0, 0.0
	};

	// index drawing data for draw a cube
	unsigned int const BACK_OFFSET = 4;
	unsigned int const LEFT_OFFSET = 8;
	unsigned int const RIGHT_OFFSET = 12;
	unsigned int const TOP_OFFSET = 16;
	unsigned int const BOTTOM_OFFSET = 20;

#ifdef USE_CUBE_CULLING
	unsigned int index_drawing_data[] = {
		// FRONT (Z-POSITIVE) specified in "clock wise" "winding order"
		0, 1, 2, /*first triangle*/
		1, 3, 2, /*second triangle*/

		// BACK (Z-NEGATIVE) specified in "counter-clock wise" "winding order"
		0 + BACK_OFFSET, 2 + BACK_OFFSET, 1 + BACK_OFFSET, /*first triangle*/
		1 + BACK_OFFSET, 2 + BACK_OFFSET, 3 + BACK_OFFSET, /*second triangle*/

		// LEFT (X-NEGATIVE) specified in "clock wise" "winding order"
		0 + LEFT_OFFSET, 1 + LEFT_OFFSET, 2 + LEFT_OFFSET, /*first triangle*/
		1 + LEFT_OFFSET, 3 + LEFT_OFFSET, 2 + LEFT_OFFSET, /*second triangle*/

		// LEFT (X-NEGATIVE) specified in "counter-clock wise" "winding order"
		0 + RIGHT_OFFSET, 2 + RIGHT_OFFSET, 1 + RIGHT_OFFSET, /*first triangle*/
		1 + RIGHT_OFFSET, 2 + RIGHT_OFFSET, 3 + RIGHT_OFFSET, /*second triangle*/

		// TOP (Y-POSITIVE) specified in "counter-clock wise" "winding order"
		0 + TOP_OFFSET, 1 + TOP_OFFSET, 2 + TOP_OFFSET, /*first triangle*/
		1 + TOP_OFFSET, 3 + TOP_OFFSET, 2 + TOP_OFFSET, /*second triangle*/

		// TOP (Y-POSITIVE) specified in "counter-clock wise" "winding order"
		0 + BOTTOM_OFFSET, 1 + BOTTOM_OFFSET, 2 + BOTTOM_OFFSET, /*first triangle*/
		1 + BOTTOM_OFFSET, 3 + BOTTOM_OFFSET, 2 + BOTTOM_OFFSET, /*second triangle*/
	};
#else
	unsigned int index_drawing_data[] = {
		// FRONT (Z-POSITIVE) specified in "clock wise" "winding order"
		0, 1, 2, /*first triangle*/
		1, 3, 2, /*second triangle*/

		// BACK (Z-NEGATIVE) specified in "clock wise" "winding order"
		1 + BACK_OFFSET, 2 + BACK_OFFSET, 0 + BACK_OFFSET, /*first triangle*/
		3 + BACK_OFFSET, 2 + BACK_OFFSET, 1 + BACK_OFFSET, /*second triangle*/

		// LEFT (X-NEGATIVE) specified in "clock wise" "winding order"
		0 + LEFT_OFFSET, 1 + LEFT_OFFSET, 2 + LEFT_OFFSET, /*first triangle*/
		1 + LEFT_OFFSET, 3 + LEFT_OFFSET, 2 + LEFT_OFFSET, /*second triangle*/

		// LEFT (X-NEGATIVE) specified in "clock wise" "winding order"
		1 + RIGHT_OFFSET, 2 + RIGHT_OFFSET, 0 + RIGHT_OFFSET, /*first triangle*/
		 3 + RIGHT_OFFSET, 2 + RIGHT_OFFSET, 1 + RIGHT_OFFSET, /*second triangle*/

		// TOP (Y-POSITIVE) specified in "clock wise" "winding order"
		2 + TOP_OFFSET, 1 + TOP_OFFSET, 0 + TOP_OFFSET, /*first triangle*/
		2 + TOP_OFFSET, 3 + TOP_OFFSET, 1 + TOP_OFFSET, /*second triangle*/

		// TOP (Y-POSITIVE) specified in "clock wise" "winding order"
		2 + BOTTOM_OFFSET, 1 + BOTTOM_OFFSET, 0 + BOTTOM_OFFSET, /*first triangle*/
		2 + BOTTOM_OFFSET, 3 + BOTTOM_OFFSET, 1 + BOTTOM_OFFSET, /*second triangle*/
	};
#endif // USE_CUBE_CULLING

	// generate VAO for store status of subsequent "vertex attribute" calls and element array buffer configs
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
	//						stride = [distance in bytes between each "position" ternas in VBO],
	//						start = [whare is the start index of "position"?];

	// indicate which part of vertex data are vertex positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); // enable the vertex attribute at location 0

	// indicate which part of vertex data are vertex colors
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1); // enable the vertex attribute at location 1

	// indicate which part of vertex data are texture coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2); // enable the vertex attribute at location 2

	std::cout << "\n\nGL_ARRAY_BUFFER:\n";
	std::cout << "sizeof(float): " << sizeof(float) << " bytes" << std::endl;
	std::cout << "Num of indices at GL_ARRAY_BUFFER: " << sizeof(vertex_data) / sizeof(float) << std::endl;
	std::cout << "Size reserved for GL_ARRAY_BUFFER: " << sizeof(vertex_data) << " bytes" << std::endl;

	// generate EBO
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	// bind EBO to GL_ELEMENT_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// pass EBO data from CPU to GPU
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_drawing_data), index_drawing_data, GL_STATIC_DRAW);

	std::cout << "\n\nGL_ELEMENT_ARRAY_BUFFER:\n";
	std::cout << "sizeof(unsigned int): " << sizeof(unsigned int) << " bytes" << std::endl;
	std::cout << "Num of indices at GL_ELEMENT_ARRAY_BUFFER: " << sizeof(index_drawing_data) / sizeof(unsigned int) << std::endl;
	std::cout << "Size reserved for GL_ELEMENT_ARRAY_BUFFER: " << sizeof(index_drawing_data) << " bytes" << std::endl;

	std::cout << "\n\nTOTAL BYTES SENT TO GPU: " << sizeof(index_drawing_data) + sizeof(vertex_data) << " bytes" << std::endl;

	// ======================================================================
	// update and draw commands

	// start viewport
	glViewport(0, 0, W, H);

	// define the winding order
	glFrontFace(GL_CW); // defines "winding order" for specify which triangle side is considered the "front" face 
#ifdef USE_CUBE_CULLING
	 glEnable(GL_CULL_FACE); // enable the face culling
	 glCullFace(GL_BACK); // cull back faces
#else
	glEnable(GL_DEPTH_TEST);
#endif // USE_CUBE_CULLING
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

	// use shader program and pass uniforms
	myShader.use();	//glUseProgram(shader_program); // use a shader program

	// locate uTime uniform
	int uTimeLoc = glGetUniformLocation(myShader.ID, "uTime");

	// specify what texture unit should use the uniform GLSL sampler uTextureA
	int uTexALoc = glGetUniformLocation(myShader.ID, "uTextureA");
	glUniform1i(uTexALoc, 0); // use texture unit 0

							  // specify what texture unit should use the uniform GLSL sampler uTextureB
	int uTexBLoc = glGetUniformLocation(myShader.ID, "uTextureB");
	glUniform1i(uTexBLoc, 1); // use texture unit 1

	// get uniform uModel, uView and uProj location at shader program
	int uModelLoc = glGetUniformLocation(myShader.ID, "uModel");
	int uViewLoc = glGetUniformLocation(myShader.ID, "uView");
	int uProjLoc = glGetUniformLocation(myShader.ID, "uProj");
	
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2, 0.5, 0.2, 1.0); // set the clear color
#ifdef USE_CUBE_CULLING
		glClear(GL_COLOR_BUFFER_BIT);
#else
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear color buffer bitfield
#endif // usecub

		process_input(window);

		float t = 0.0f;// (float)glfwGetTime();
		float s = 0.5 + 0.5 * sin((float)glfwGetTime());

		glUniform1f(uTimeLoc, t);

		// create model matrix
		glm::mat4 model;
		model = glm::rotate(model, t, glm::vec3(1.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, t, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uModelLoc, 1, GL_FALSE, glm::value_ptr(model)); // GLint location,	GLsizei count,	GLboolean transpose, const GLfloat* value

		// create view matrix
		glm::mat4 view = glm::mat4(1.0f);

		//// Rotates camera around cameraTarget
		//const float radius = 5.0f;
		//float camX = sin(glfwGetTime()) * radius;
		//float camZ = cos(glfwGetTime()) * radius;
		//cameraPos.x = camX;
		//cameraPos.z = camZ;

		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp); // Generates a Look At matrix

		glUniformMatrix4fv(uViewLoc, 1, GL_FALSE, glm::value_ptr(view));

		// creat projection matrix
		glm::mat4 projection;
		//projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);
		projection = glm::perspective(glm::radians(45.0f), (float)(W / H), 0.1f, 100.0f);
		glUniformMatrix4fv(uProjLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO); // bind object VAO
		
		// read more at: https://people.eecs.ku.edu/~jrmiller/Courses/672/InClass/3DModeling/glDrawElements.html
		// glDrawArrays(GL_TRIANGLES, 0, 3); // draw triangle
		const int VERTICES_PER_TRIANGLE = 3;
		const int NUM_OF_TRIANGLES = 12;
		GLenum mode = GL_TRIANGLES; // Specifies what kind of primitives to render.
		GLsizei count = VERTICES_PER_TRIANGLE * NUM_OF_TRIANGLES; // Specifies the number of elements to be rendered.
		GLenum type = GL_UNSIGNED_INT; // Specifies the type of the values in indices.Must be one of GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, or GL_UNSIGNED_INT.
		GLvoid * indices = nullptr; // Specifies a pointer to the location where the indices are stored
		// Passing nullptr as the final parameter to glDrawElements tells the vertex fetch processor to use the currently bound element buffer object when extracting per - vertex data for vertex shader executions.
		glDrawElements(mode, count, type, indices); // draw a quad

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
	const float radius = 3.0f;
	cameraPos.x = cos(PI * cameraPosT) * radius;
	cameraPos.z = sin(PI * cameraPosT) * radius;

	glm::vec3 lookAt(0.0f);
	cameraFront = glm::normalize(lookAt - cameraPos);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPosT -= cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPosT += cameraSpeed;
}