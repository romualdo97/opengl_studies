// use winding order to draw correctly cube faces? else use depth testing method
#define NR_POINT_LIGHTS 3
#define NR_SPOT_LIGHTS 2

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

// data for drawing point and spot lights
#include "PointLights.h"
#include "SpotLights.h"

#define W 820
#define H 820
#define WINDOW_TITLE "Animating mesh using matrix transformations"

void resize_framebuffer_cb(GLFWwindow *window, int w, int h);
void process_input(GLFWwindow *window);
unsigned int load_texture(const char * srcToTexture);

glm::vec3 cameraPos(7.0f);

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

	Shader surfaceShader("Shaders/surface"); // Shader program for object that will be illuminated
	Shader lightShader("Shaders/light"); // Shader program for light source

	// ======================================================================
	// send data to GPU for drawing a box
	Box box;
	glm::vec3 boxPositions[] = {
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

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2, 0.5, 0.2, 1.0); // set the clear color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear color buffer bitfield

		process_input(window);

		// Just for animating
		float t = 0.0f; // (float)glfwGetTime();
		float s = 0.5f + 0.5f * sin((float)glfwGetTime());

		// Directional light color
		glm::vec3 lightColor = glm::vec3(1.0f);

		// Darken the light intensity
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.2f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.1f);

		// ======================================================================
		// Draw box that will be illuminated

		// slect shader program and pass uniforms
		surfaceShader.use();	//glUseProgram(shader_program); // use a shader program
		glBindVertexArray(box.VAO); // bind box VAO

		// create view matrix
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(cameraPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Generates a Look At matrix // note that we're translating the scene in the reverse direction of where we want to move
		surfaceShader.setMatrix("uView", view);

		// create projection matrix
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), (float)(W / H), 0.1f, 100.0f);
		surfaceShader.setMatrix("uProj", projection);

		// pass material info
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMapTexture);
		surfaceShader.setInt("uMaterial.diffuse", 0); // Diffuse uniform will fetch sampler from the texture unit 0

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMapTexture);
		surfaceShader.setInt("uMaterial.specular", 1); // Specular uniform will fetch sampler from the texture unit 1

		surfaceShader.setFloat("uMaterial.shininess", 3.0f);

		// pass camera position
		surfaceShader.setVec3("uViewPos", cameraPos);

		// pass directional light info
		surfaceShader.setVec3("uDirectionalLight.direction", -1.0f, -1.0f, 0.0f);
		surfaceShader.setVec3("uDirectionalLight.ambient", ambientColor);
		surfaceShader.setVec3("uDirectionalLight.diffuse", diffuseColor);
		surfaceShader.setVec3("uDirectionalLight.specular", 1.0f, 1.0f, 1.0f);

		// 32bytes buffer for passing info to shader
		char finalUniformName[32]; 

		// pass point lights info
		for (int i = 0; i < NR_POINT_LIGHTS; ++i)
		{
			// Pass position
			snprintf(finalUniformName, 64, "uPointLights[%d].position", i);
			surfaceShader.setVec3(finalUniformName, pointLightPositions[i]);

			// Pass constant term
			snprintf(finalUniformName, 64, "uPointLights[%d].constant", i);
			surfaceShader.setFloat(finalUniformName, pointLightConstants[i]);

			// Pass linear term
			snprintf(finalUniformName, 64, "uPointLights[%d].linear", i);
			surfaceShader.setFloat(finalUniformName, pointLightLinears[i]);

			// Pass quadratic term
			snprintf(finalUniformName, 64, "uPointLights[%d].quadratic", i);
			surfaceShader.setFloat(finalUniformName, pointLightQuadratics[i]);

			// Pass point light colors
			glm::vec3 lightColor = pointLightColors[i];
			glm::vec3 diffuseColor = lightColor * glm::vec3(1.0f);
			glm::vec3 ambientColor = diffuseColor * glm::vec3(1.0f);

			snprintf(finalUniformName, 64, "uPointLights[%d].ambient", i);
			surfaceShader.setVec3(finalUniformName, ambientColor);

			snprintf(finalUniformName, 64, "uPointLights[%d].diffuse", i);
			surfaceShader.setVec3(finalUniformName, diffuseColor);

			snprintf(finalUniformName, 64, "uPointLights[%d].specular", i);
			surfaceShader.setVec3(finalUniformName, 1.0f, 1.0f, 1.0f);
		}

		// pass spot lights info
		for (int i = 0; i < NR_SPOT_LIGHTS; ++i)
		{
			// Pass position
			snprintf(finalUniformName, 64, "uSpotLights[%d].position", i);
			surfaceShader.setVec3(finalUniformName, spotLightPositions[i]);

			// Pass direction
			snprintf(finalUniformName, 64, "uSpotLights[%d].direction", i);
			surfaceShader.setVec3(finalUniformName, spotLightDirections[i]);

			// Pass cutoff
			snprintf(finalUniformName, 64, "uSpotLights[%d].cutOff", i);
			surfaceShader.setFloat(finalUniformName, glm::cos(glm::radians(spotLightCutOffs[i])));

			// Pass outerCutOff
			snprintf(finalUniformName, 64, "uSpotLights[%d].outerCutOff", i);
			surfaceShader.setFloat(finalUniformName, glm::cos(glm::radians(spotLightOuterCutOffs[i])));

			// Pass point light colors
			glm::vec3 lightColor = spotLightColors[i];
			glm::vec3 diffuseColor = lightColor * glm::vec3(0.8f);
			glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

			snprintf(finalUniformName, 64, "uSpotLights[%d].ambient", i);
			surfaceShader.setVec3(finalUniformName, ambientColor);

			snprintf(finalUniformName, 64, "uSpotLights[%d].diffuse", i);
			surfaceShader.setVec3(finalUniformName, diffuseColor);

			snprintf(finalUniformName, 64, "uSpotLights[%d].specular", i);
			surfaceShader.setVec3(finalUniformName, 1.0f, 1.0f, 1.0f);
		}

		// Draw a box
		for (int i = 0; i < 4; ++i)
		{
			// Prepare the model matrix
			box.model = glm::mat4(1.0f); //glm::mat4 model;
			box.setPosition(boxPositions[i]);
			box.setRotationY(t);
			
			// Set the model matrix
			surfaceShader.setMatrix("uModel", box.model);

			// Rasterize vertices based on current OpenGL state machine state
			box.draw();
		}

		// ======================================================================
		// Draw box for the light source

		// select shader program and pass uniforms
		lightShader.use(); // glUseProgram(shader_program); // use a shader program
		// glBindVertexArray(box.VAO); // bind box VAO // let's use same box VAO

		lightShader.setMatrix("uView", view);
		lightShader.setMatrix("uProj", projection);

		for (int i = 0; i < NR_POINT_LIGHTS; ++i)
		{
			// Calculate box position
			box.model = glm::mat4(1.0f);
			box.setPosition(pointLightPositions[i]);
			box.model = glm::scale(box.model, glm::vec3(0.2f)); // Scale down light

			// pass point light color
			lightShader.setVec3("uLightColor", pointLightColors[i]);
			
			// pass model matrix
			lightShader.setMatrix("uModel", box.model);

			// draw another box for the light source
			box.draw();
		}
		
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
	// Update first spot light based on camera position
	spotLightPositions[0].x = cameraPos.x;
	spotLightPositions[0].y = cameraPos.y;
	spotLightPositions[0].z = cameraPos.z;

	// Make spot light point to origin
	spotLightDirections[0] = glm::normalize(-cameraPos);
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