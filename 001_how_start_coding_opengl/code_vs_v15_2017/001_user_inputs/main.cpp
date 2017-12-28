#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <iostream>

// callback for window size modification
void framebuffer_size_cb(GLFWwindow *window, int width, int heigth);

// process keyboar inputs
void process_input(GLFWwindow *window);

int main(void)
{
	
	// =========================================================================
	// 1. INIT GLFW AND PREPARE WINDOW AND CONTEXT CREATION
	glfwInit();								// init glfw
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	// request to create context for opengl 3.x
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);	// request to create context for opengl x.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // request to create core profile context
	// =========================================================================

	// =========================================================================
	// 2. CREATE WINDOW AND MAKE ITS CONTEXT THE CURRENT
	GLFWwindow *window = glfwCreateWindow(980, 640, "Basic GLFW input/output system", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Window creation failed!\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// =========================================================================

	// =========================================================================
	// 3. MANAGES OPENGL FUNCTIONS POINTERS
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD\n";
		return -1;
	}
	// =========================================================================
	
	// =========================================================================
	// 4. REGISSTER GLFW EVENTS
	glfwSetFramebufferSizeCallback(window, framebuffer_size_cb);
	// =========================================================================

	glViewport(0, 0, 980, 640);

	while(!glfwWindowShouldClose(window))
	{
		process_input(window); // process keyboard inputs

		glClearColor(0.5, 0.7, 0.5, 1.0); // set the clear color
		glClear(GL_COLOR_BUFFER_BIT); // clear the color buffer bitfield

		glfwSwapBuffers(window); // swap opengl back and front bufers
		glfwPollEvents(); // listen for events
	}
	
	glfwTerminate();
	return 0;
}

void framebuffer_size_cb(GLFWwindow *window, int width, int heigth)
{
	glViewport(0, 0, width, heigth);
}

void process_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(window, true);
	}
}