// use winding order to draw correctly cube faces? else use depth testing method
//#define USE_BASIC_POINT_LIGHT
//#define USE_DIRECTIONAL_LIGHT
//#define USE_POINT_LIGHT
#define USE_SPOT_LIGHT

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
unsigned int load_texture(const char * srcToTexture);

glm::vec3 cameraPos(7.0f);
glm::vec3 lightPos(1.2f, 1.0f, 0.0f);

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
	// surfaceBasicPointLight, surfaceDirectionalLight

#ifdef USE_BASIC_POINT_LIGHT
	Shader surfaceShader("Shaders/surface.vert", "Shaders/surfaceBasicPointLight.frag"); // Shader program for object that will be illuminated
#endif

#ifdef USE_DIRECTIONAL_LIGHT
	Shader surfaceShader("Shaders/surface.vert", "Shaders/surfaceDirectionalLight.frag"); // Shader program for object that will be illuminated
	glm::vec3 lightDir(0.0f, 0.0f, -1.0f);
#endif

#ifdef USE_POINT_LIGHT
	Shader surfaceShader("Shaders/surface.vert", "Shaders/surfacePointLight.frag"); // Shader program for object that will be illuminated
#endif

#ifdef USE_SPOT_LIGHT
	Shader surfaceShader("Shaders/surface.vert", "Shaders/surfaceSpotLight.frag"); // Shader program for object that will be illuminated
#endif

	Shader lightShader("Shaders/light"); // Shader program for light source

	// ======================================================================
	// send data to GPU for drawing a box
	Box box;
	glm::vec3 positions[] = {
		glm::vec3(0, 0, 0),	glm::vec3(1, -1, -4),
		glm::vec3(-1, -3, -3), glm::vec3(1, -2, -6)
	};

	// load diffuse and specular maps
	unsigned int diffuseMapTexture = load_texture("container2.png");
	unsigned int specularMapTexture = load_texture("container2_specular.png");

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

	// Material related
	int uMaterialDiffuseLoc = glGetUniformLocation(surfaceShader.ID, "uMaterial.diffuse");
	int uMaterialSpecularLoc = glGetUniformLocation(surfaceShader.ID, "uMaterial.specular");
	int uMaterialShininessLoc = glGetUniformLocation(surfaceShader.ID, "uMaterial.shininess");

	// Light related
	int uViewPosLoc = glGetUniformLocation(surfaceShader.ID, "uViewPos");

#ifdef USE_BASIC_POINT_LIGHT
	int uLightPosLoc = glGetUniformLocation(surfaceShader.ID, "uLight.position");
#endif

#ifdef USE_DIRECTIONAL_LIGHT
	int uLightDirLoc = glGetUniformLocation(surfaceShader.ID, "uLight.direction");
#endif

#ifdef USE_POINT_LIGHT
	int uLightPosLoc = glGetUniformLocation(surfaceShader.ID, "uLight.position");
	int uLightConstantLoc = glGetUniformLocation(surfaceShader.ID, "uLight.constant");
	int uLightLinearLoc = glGetUniformLocation(surfaceShader.ID, "uLight.linear");
	int uLightQuadraticLoc = glGetUniformLocation(surfaceShader.ID, "uLight.quadratic");
#endif

#ifdef USE_SPOT_LIGHT
	int uLightPosLoc = glGetUniformLocation(surfaceShader.ID, "uLight.position");
	int uLightDirLoc = glGetUniformLocation(surfaceShader.ID, "uLight.direction");
	int uLightCutOffLoc = glGetUniformLocation(surfaceShader.ID, "uLight.cutOff");
	int uLightOuterCutOffLoc = glGetUniformLocation(surfaceShader.ID, "uLight.outerCutOff");
#endif

	int uLightAmbientLoc = glGetUniformLocation(surfaceShader.ID, "uLight.ambient");
	int uLightDiffuseLoc = glGetUniformLocation(surfaceShader.ID, "uLight.diffuse");
	int uLightSpecularLoc = glGetUniformLocation(surfaceShader.ID, "uLight.specular");

	// get uniform uModel, uView and uProj location at shader program
	int uModelLoc = glGetUniformLocation(surfaceShader.ID, "uModel");
	int uViewLoc = glGetUniformLocation(surfaceShader.ID, "uView");
	int uProjLoc = glGetUniformLocation(surfaceShader.ID, "uProj");

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
		float t = 0.0f; // (float)glfwGetTime();
		float s = 0.5f + 0.5f * sin((float)glfwGetTime());

		// Animate light color
		glm::vec3 lightColor = glm::vec3(1.0f);
		/*lightColor.x = 0.5f * sin(glfwGetTime() * 2.0f) + 0.5f;
		lightColor.y = 0.5f * sin(glfwGetTime() * 0.7f) + 0.5f;
		lightColor.z = 0.5f * sin(glfwGetTime() * 1.3f) + 0.5f;*/

		// Darken the light intensity
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

		// ======================================================================
		// Draw box that will be illuminated

		// slect shader program and pass uniforms
		surfaceShader.use();	//glUseProgram(shader_program); // use a shader program
		glBindVertexArray(box.VAO); // bind box VAO

		// create view matrix
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(cameraPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Generates a Look At matrix // note that we're translating the scene in the reverse direction of where we want to move
		glUniformMatrix4fv(uViewLoc, 1, GL_FALSE, glm::value_ptr(view));

		// creat projection matrix
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), (float)(W / H), 0.1f, 100.0f);
		glUniformMatrix4fv(uProjLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// pass material info
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMapTexture);
		glUniform1i(uMaterialDiffuseLoc, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMapTexture);
		glUniform1i(uMaterialSpecularLoc, 1);

		glUniform1f(uMaterialShininessLoc, 32.0f);

		// pass light info
		glUniform3fv(uViewPosLoc, 1, glm::value_ptr(cameraPos));

#ifdef USE_BASIC_POINT_LIGHT
		glUniform3fv(uLightPosLoc, 1, glm::value_ptr(lightPos));
#endif
#ifdef USE_DIRECTIONAL_LIGHT
		glUniform3fv(uLightDirLoc, 1, glm::value_ptr(lightDir));
#endif
#ifdef USE_POINT_LIGHT
		// Attenuation coefficients
		// https://www.desmos.com/calculator/wtt73x5kwp
		// https://learnopengl.com/Lighting/Light-casters
		glUniform1f(uLightConstantLoc, 1.0f);
		glUniform1f(uLightLinearLoc, 0.0f);
		glUniform1f(uLightQuadraticLoc, 0.3f);
		glUniform3fv(uLightPosLoc, 1, glm::value_ptr(lightPos));
#endif

#ifdef USE_SPOT_LIGHT
		glUniform3fv(uLightPosLoc, 1, glm::value_ptr(cameraPos));
		glUniform3fv(uLightDirLoc, 1, glm::value_ptr(glm::normalize(-cameraPos)));
		glUniform1f(uLightCutOffLoc, glm::cos(glm::radians(12.0f)));
		glUniform1f(uLightOuterCutOffLoc, glm::cos(glm::radians(15.0f)));
#endif

		glUniform3fv(uLightAmbientLoc, 1, &(ambientColor.x));
		glUniform3fv(uLightDiffuseLoc, 1, &(diffuseColor.x)); // darken diffuse light a bit
		glUniform3f(uLightSpecularLoc, 1.0f, 1.0f, 1.0f);

		// Draw a box
		for (int i = 0; i < 4; ++i)
		{
			box.model = glm::mat4(1.0f); //glm::mat4 model;
			box.setPosition(positions[i]);
			box.setRotationY(t); //model = glm::rotate(model, t, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uModelLoc, 1, GL_FALSE, glm::value_ptr(box.model)); // GLint location,	GLsizei count,	GLboolean transpose, const GLfloat* value
			box.draw();
		}

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
}

unsigned int load_texture(const char* srcToTexture)
{
	// tell stb library to flip images in load
	stbi_set_flip_vertically_on_load(true);

	// load texture data using stb_image header library
	int image_width, image_height, nrChannels;
	unsigned char* image_data = stbi_load(srcToTexture, &image_width, &image_height, &nrChannels, 0);

	// Generate OpenGL texture object
	unsigned int texture;
	glGenTextures(1, &texture);

	// bind for configuration
	glActiveTexture(GL_TEXTURE0); // activate texture unit 0
	glBindTexture(GL_TEXTURE_2D, texture);

	// put imge_data to previously created opengl texture object
	if (image_data)
	{
		if (nrChannels == 3) // jpg
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
		}
		else if (nrChannels == 4) // png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Error loading texture data\n";
	}

	// unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}