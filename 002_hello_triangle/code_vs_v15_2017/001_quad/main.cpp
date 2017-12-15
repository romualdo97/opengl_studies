#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>

#define TITLE "Quad drawing"
#define W 960
#define H 860

char const *vertex_shader_source = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main(){\n"
"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\n";

char const *fragment_shader_source = "#version 330 core\n"
"out vec4 FragColor;;\n"
"void main(){\n"
"FragColor = vec4(1.0f, 0.8f, 0.2f, 1.0f);\n"
"}\n";

int main(void)
{
	// init glfw, configure window and its opengl context
	glfwInit();
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // let´s use core profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // let´s use opengl 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// create window
	GLFWwindow *window = glfwCreateWindow(W, H, TITLE, NULL, NULL);
	if (NULL == window)
	{
		std::cout << "Window not created\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// load opengl functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "GLAD not initialized\n";
		return -1;
	}

	// compile vertex shader
	unsigned int vertex_shader;
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);
	char infolog[512];
	int status;
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);
	if (!status)
	{
		glGetShaderInfoLog(vertex_shader, sizeof(infolog), NULL, infolog);
		std::cout << "SHADER::VERTEX_SHADER::COMPILATION_ERROR\n";
		std::cout << infolog << std::endl;
	}

	// compile fragmente shader
	unsigned int fragment_shader;
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &status);
	if (!status)
	{
		glGetShaderInfoLog(fragment_shader, sizeof(infolog), NULL, infolog);
		std::cout << "SHADER::FRAGMENT_SHADER::COMPILATION_ERROR\n";
		std::cout << infolog << std::endl;
	}

	// create shader program
	unsigned int shader_program;
	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);
	glGetProgramiv(shader_program, GL_LINK_STATUS, &status);
	if (!status)
	{
		glGetProgramInfoLog(shader_program, sizeof(infolog), NULL, infolog);
		std::cout << "SHADER::PROGRAM::ERROR_LINKING\n";
		std::cout << infolog << std::endl;
	}

	// quad vertex data in clock-wise orientation
	float vertices[] = {
		0.5, 0.5, 0.0, // right top
		0.5, -0.5, 0.0, // righ bottom
		-0.5, -0.5, 0.0, // left bottom
		-0.5, 0.5, 0.0 // left top
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	// create VAO for store opengl status
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	// generate buffers
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind VBO TO GL_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // bind EBO to GL_ELEMENT_ARRAY_BUFFER
	// set buffer data
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // copy data from CPU to GPU
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // set indices in appropiate buffer for index drawing
	// assign "sense" to data in VBO buffer
	glVertexAttribPointer(0/*layout(location=0)*/, 3/*vec3*/, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0/*start index*/);
	glEnableVertexAttribArray(0);

	glViewport(0, 0, W, H);

	while(!glfwWindowShouldClose(window))
	{
		// background
		glClearColor(0.5, 0.7, 0.5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader_program);
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
		// poll events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}