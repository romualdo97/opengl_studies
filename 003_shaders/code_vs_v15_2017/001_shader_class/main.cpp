#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <iostream>

#define W 960
#define H 480
#define WINDOW_TITLE "My triangle exercise"

void resize_framebuffer_cb(GLFWwindow *window, int w, int h);
void process_input(GLFWwindow *window);

char const *vertex_shader_src = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 vColor;\n"
"void main() { \n"
"   gl_Position = vec4(vec3(aPos), 1.0);\n"
"	vColor = aColor;\n"
"}\n";

char const *fragment_shader_src = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 vColor;\n"
"uniform vec3 uColor;\n"
"void main() {\n"
"	FragColor = vec4(vColor, 1.0);\n"
"}\n";


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

	// generate vertex shader
	unsigned int vertex_shader;
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	// attach the shader source code to shader object
	glShaderSource(vertex_shader, 1, &vertex_shader_src, NULL);
	glCompileShader(vertex_shader); // compile shader

	// check for compile-time errors
	int succes;
	char infolog[512];
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &succes);
	if (!succes)
	{
		glGetShaderInfoLog(vertex_shader, sizeof(infolog), NULL, infolog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infolog << "\n";
	}

	// generate fragment shader
	unsigned int fragment_shader;
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER); // create shader
	// attach shader source code to shader object
	glShaderSource(fragment_shader, 1, &fragment_shader_src, NULL);
	glCompileShader(fragment_shader); // compile shader

	// check compile-time errors
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &succes);
	if (!succes)
	{
		glGetShaderInfoLog(fragment_shader, sizeof(infolog), NULL, infolog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infolog << "\n";
	}

	// generate shader program object
	unsigned int shader_program;
	shader_program = glCreateProgram();
	// attach the previuosly compiled shaders to shader program object
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program); // link shaders

	// check linking errors
	glGetProgramiv(shader_program, GL_LINK_STATUS, &succes);
	if (!succes)
	{
		glGetProgramInfoLog(shader_program, sizeof(infolog), NULL, infolog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infolog << "\n";
	}

	// once linked, we no longer need vertex/fragment shader objects
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	
	// ======================================================================
	
	// triangle vertex data 
	float vertex_data[] = {
		// positions (in NDC)				// colors
		-0.3f, 0.5f, 0.0f,	/*left-top*/	1.0f, 0.0f, 0.0f,	/*red*/
		0.3f, 0.5f, 0.0f,	/*right-top*/	0.0f, 1.0f, 0.0f,	/*green*/
		0.0f, -0.5f, 0.0f,	/*botton*/		0.0f, 0.0f, 1.0f	/*blue*/
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0); // enable the vertex attribute at location 0

	// indicate which part of vertex data are vertex colors
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1); // enable the vertex attribute at location 1
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
		float g = 0.5 + 0.5 * sin(t);
		int uColorLoc = glGetUniformLocation(shader_program, "uColor");

		glUseProgram(shader_program); // use a shader program
		glUniform3f(uColorLoc, 1 - g, g, 0.0f);

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