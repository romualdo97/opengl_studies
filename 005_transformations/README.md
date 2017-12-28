# passing matrices as uniforms from cpu to gpu

	unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

The first argument should be familiar by now which is the uniform's location. The second argument tells OpenGL how many matrices we'd like to send, which is 1. The third argument asks us if we want to transpose our matrix, that is to swap the columns and rows. OpenGL developers often use an internal matrix layout called column-major ordering which is the default matrix layout in GLM so there is no need to transpose the matrices; we can keep it at GL_FALSE. The last parameter is the actual matrix data, but GLM stores their matrices not in the exact way that OpenGL likes to receive them so we first transform them with GLM's built-in function value_ptr.

> Using transformations in the vertex shader saves us the effort of re-defining the vertex data and saves us some processing time as well, since we don't have to re-send our data all the time (which is quite slow).

# Homogeneous coordinates
The w component of a vector is also known as a **homogeneous coordinate**. To get the 3D vector from a homogeneous vector we divide the x, y and z coordinate by its w coordinate. We usually do not notice this since the w component is 1.0 most of the time. Using homogeneous coordinates has several advantages: it allows us to do translations on 3D vectors (without a w component we can't translate vectors).

Also, whenever the homogeneous coordinate is equal to 0 the vector is specifically known as a direction vector since a vector with a w coordinate of 0 cannot be translated.

# Non-uniform scaling
When factor scale of a **vector scale operation** is not the same for each component

> learn more [here](https://learnopengl.com/#!Getting-started/Transformations).