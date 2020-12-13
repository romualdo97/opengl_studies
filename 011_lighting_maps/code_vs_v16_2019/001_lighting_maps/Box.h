#pragma once

// index drawing data for draw a cube
unsigned int const BACK_OFFSET = 4;
unsigned int const LEFT_OFFSET = 8;
unsigned int const RIGHT_OFFSET = 12;
unsigned int const TOP_OFFSET = 16;
unsigned int const BOTTOM_OFFSET = 20;

// Struct used for drawwing boxes
struct Box
{
public:

	Box()
	{
		// generate VAO for store status of subsequent "vertex attribute" calls and element array buffer configs
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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0); // enable the vertex attribute at location 0

		// indicate which part of vertex data are vertex colors
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1); // enable the vertex attribute at location 1

		// indicate which part of vertex data are texture coordinates
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2); // enable the vertex attribute at location 2

		// indicate which part of vertex data are model normals
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
		glEnableVertexAttribArray(3); // enable the vertex attribute at location 2

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
	}

	void draw()
	{
		// define the winding order
#ifdef USE_CUBE_CULLING
		glFrontFace(GL_CW); // defines "winding order" for specify which triangle side is considered the "front" face 
		glEnable(GL_CULL_FACE); // enable the face culling
		glCullFace(GL_BACK); // cull back faces
#else
		glEnable(GL_DEPTH_TEST);
#endif // USE_CUBE_CULLING

		// read more at: https://people.eecs.ku.edu/~jrmiller/Courses/672/InClass/3DModeling/glDrawElements.html
		// glDrawArrays(GL_TRIANGLES, 0, 3); // draw triangle
		const int VERTICES_PER_TRIANGLE = 3;
		const int NUM_OF_TRIANGLES = 12;
		GLenum mode = GL_TRIANGLES; // Specifies what kind of primitives to render.
		GLsizei count = VERTICES_PER_TRIANGLE * NUM_OF_TRIANGLES; // Specifies the number of elements to be rendered.
		GLenum type = GL_UNSIGNED_INT; // Specifies the type of the values in indices.Must be one of GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, or GL_UNSIGNED_INT.
		GLvoid* indices = nullptr; // Specifies a pointer to the location where the indices are stored

		// Passing nullptr as the final parameter to glDrawElements tells the vertex fetch processor to use the currently bound element buffer object when extracting per - vertex data for vertex shader executions.
		glDrawElements(mode, count, type, indices); // draw a quad
	}

	void setPosition(glm::vec3 position)
	{
		model = glm::translate(model, position);
	}

	void setRotationY(float radians)
	{
		model = glm::rotate(model, radians, glm::vec3(0.0f, 1.0f, 0.0f));
	}


	// Initialized using default memeber initilization to solve warning https://docs.microsoft.com/en-us/cpp/code-quality/c26495?view=msvc-160
	unsigned int VAO{};
	unsigned int VBO{};
	unsigned int EBO{};

	// Related to model matrix
	glm::mat4 model;

	// Data for sending to GPU
	const float vertex_data[11 * 4 * 6] = {
		// FRONT (Z-POSITIVE)
		// positions (in NDC)				// colors						// texture coordinates		// face normal
		-0.5f, 0.5f, 0.5f,	/*left-top*/	0.0f, 0.0f, 1.0f,	/*blue*/	0.0, 1.0,					0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f,	/*right-top*/	0.0f, 0.0f, 1.0f,	/*blue*/	1.0, 1.0,					0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f,	/*left-bttm*/	0.0f, 0.0f, 1.0f,	/*blue*/	0.0, 0.0,					0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f,	/*right-bttm*/  0.0f, 0.0f, 1.0f,	/*blue*/	1.0, 0.0,					0.0f, 0.0f, 1.0f,

		// BACK (Z-NEGATIVE)
		// positions (in NDC)				// colors						// texture coordinates		// face normal
		-0.5f, 0.5f, -0.5f,	/*left-top*/	0.0f, 0.0f, 0.5f,	/*blue*/	0.0, 1.0,					0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, -0.5f,	/*right-top*/	0.0f, 0.0f, 0.5f,	/*blue*/	1.0, 1.0,					0.0f, 0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,/*left-bttm*/	0.0f, 0.0f, 0.5f,	/*blue*/	0.0, 0.0,					0.0f, 0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,	/*right-bttm*/  0.0f, 0.0f, 0.5f,	/*blue*/	1.0, 0.0,					0.0f, 0.0f, -1.0f,

		// LEFT (X-NEGATIVE)
		// positions (in NDC)				// colors						// texture coordinates		// face normal
		-0.5f, 0.5f, -0.5f,	/*left-top*/	0.5f, 0.0f, 0.0f,	/*red*/		0.0, 1.0,					-1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f,	/*right-top*/	0.5f, 0.0f, 0.0f,	/*red*/		1.0, 1.0,					-1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, /*left-bttm*/	0.5f, 0.0f, 0.0f,	/*red*/		0.0, 0.0,					-1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, /*right-bttm*/  0.5f, 0.0f, 0.0f,	/*red*/		1.0, 0.0,					-1.0f, 0.0f, 0.0f,

		// RIGHT (X-POSITIVE)
		// positions (in NDC)				// colors						// texture coordinates		// face normal
		0.5f, 0.5f, -0.5f,	/*left-top*/	1.0f, 0.0f, 0.0f,	/*red*/		0.0, 1.0,					1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f,	/*right-top*/	1.0f, 0.0f, 0.0f,	/*red*/		1.0, 1.0,					1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, /*left-bttm*/	1.0f, 0.0f, 0.0f,	/*red*/		0.0, 0.0,					1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, /*right-bttm*/   1.0f, 0.0f, 0.0f,	/*red*/		1.0, 0.0,					1.0f, 0.0f, 0.0f,

		// TOP (Y-POSITIVE)
		// positions (in NDC)				// colors						// texture coordinates		// face normal
		-0.5f, 0.5f, -0.5f,	/*left-top*/	0.0f, 1.0f, 0.0f,	/*green*/	0.0, 1.0,					0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f,	/*right-top*/	0.0f, 1.0f, 0.0f,	/*green*/	1.0, 1.0,					0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, /*left-bttm*/	0.0f, 1.0f, 0.0f,	/*green*/	0.0, 0.0,					0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, /*right-bttm*/    0.0f, 1.0f, 0.0f,	/*green*/	1.0, 0.0,					0.0f, 1.0f, 0.0f,

		// BOTTOM (Y-NEGATIVE)
		// positions (in NDC)				// colors						// texture coordinates		// face normal
		-0.5f, -0.5f, -0.5f,/*left-top*/	0.0f, 0.5f, 0.0F,	/*green*/	0.0, 1.0,					0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f,	/*right-top*/	0.0f, 0.5f, 0.0f,	/*green*/	1.0, 1.0,					0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, -0.5f, /*left-bttm*/	0.0f, 0.5f, 0.0f,	/*green*/	0.0, 0.0,					0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, /*right-bttm*/   0.0f, 0.5f, 0.0f,	/*green*/	1.0, 0.0,					0.0f, -1.0f, 0.0f,
	};
	const unsigned int index_drawing_data[3 * 2 * 6] = {
#ifdef USE_CUBE_CULLING
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
#else
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
#endif
	};
};

