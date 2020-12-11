#pragma once

#include <glad\glad.h>

// Struct used for drawwing quads
struct Quad
{
public:

	Quad()
	{
		// generate VAO for store status of subsequent "vertex attribute" calls
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO); // bind VAO

		// generate VBO for allocate memory in GPU
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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0); // enable the vertex attribute at location 0

		// indicate which part of vertex data are vertex colors
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1); // enable the vertex attribute at location 1

		// indicate which part of vertex data are texture coordinates
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2); // enable the vertex attribute at location 2

		// generate EBO
		glGenBuffers(1, &EBO);
		// bind EBO to GL_ELEMENT_ARRAY_BUFFER
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		// pass EBO data from CPU to GPU
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_drawing_data), index_drawing_data, GL_STATIC_DRAW);
	}

	void draw()
	{
		glBindVertexArray(VAO); // bind object VAO
		//glDrawArrays(GL_TRIANGLES, 0, 3); // draw triangle
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // draw a quad
	}

	// Initialized using default memeber initilization to solve warning https://docs.microsoft.com/en-us/cpp/code-quality/c26495?view=msvc-160
	unsigned int VAO{};
	unsigned int VBO{};
	unsigned int EBO{};

	// quad vertex data in clock-wise orientation
	float vertex_data[8 * 4] = {
		// positions (in NDC)				// colors						// texture coordinates
		-1.0f, 1.0f, 0.0f,	/*left-top*/	1.0f, 0.0f, 0.0f,	/*red*/		0.0, 1.0,
		1.0f, 1.0f, 0.0f,	/*right-top*/	0.0f, 1.0f, 0.0f,	/*green*/	1.0, 1.0,
		-1.0f, -1.0f, 0.0f,	/*left-bttm*/	0.0f, 0.0f, 1.0f,	/*blue*/	0.0, 0.0,
		1.0f, -1.0f, 0.0f,	/*right-bttm*/  0.0f, 1.0f, 1.0f,	/*#0ff*/	1.0, 0.0
	};

	unsigned int index_drawing_data[3 * 2] = {
		0, 1, 2,
		1, 3, 2,
	};
};