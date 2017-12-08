#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <iostream>

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
	GLFWwindow *window = glfwCreateWindow(980, 640, "Hello Window", NULL, NULL);
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
	
	glViewport(0, 0, 980, 640);

	while(!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window); // swap opengl back and front bufers
		glfwPollEvents(); // listen for events
	}
	
	glfwTerminate();
	return 0;
}