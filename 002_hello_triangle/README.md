# Warning
Never forget the `in` keyword of GLSL for the `vertex attributes`.
the next won´t work when configuring VBO data for point to the vertex attribute in the vertex shader (using `glVertexAttribPointer` function).

	layout (location = 0) vec3 attribute_position;
This is the correct method, using the `in` keyword for specify some like the `attribute` in old GLSL.

	layout (location = 0) in vec3 attribute_position;

In old GLSL this would look like:

	layour (location = 0) attribute vec3 position;

# About VAO, VBO y EBO

![enter image description here](https://i.imgur.com/QEYMZlL.png)

El **VAO** tiene **attribute pointers** que sirven de referencia a **vertex attributes** del **vertex shader**

Se pasan datos de la CPU a la GPU usando los **VBO** cuyo `gl_target` es `GL_ARRAY_BUFFER`, estos datos pueden ser cualquier cosa, desde posiciones a colores o tiempo.

El significado de un dato o conjunto de datos en el **VBO** (`GL_ARRAY_BUFFER`) depende de a que **vertex attribute** del **vertex shader** apunta dicho dato o conjunto de datos.

La forma en que se configura a que **vertex attribute** del **vertex shader** apunta un dato del **VBO** es mediante la función **glVertexAtrribPointer**

Un **VAO** (Vertex Array Object) tambien puede registrar el estado de un **EBO** (Element Buffer Object)

El **EBO** es muy similar al **VBO** pero el target del EBO es `GL_ELEMENT_ARRAY_BUFFER` mientras que el del VBO es `GL_ARRAY_BUFFER`

El **EBO** tambien sirver para pasar datos desde la CPU a la GPU, solo que estos **datos** son especiales

los datos del **EBO** son especiales porque a diferencia de aquellos del **VBO** los del EBO no sirven para apuntar a ningún **vertex attribute** del **vertex shader**

sin embargo los datos del **EBO** sirven para indicar mediante **indices** el "orden" en que la pipeline gráfica debe dibujar un conjunto de vértices en el **VBO**. (leer acerca del [post-transform cache](https://www.khronos.org/opengl/wiki/Post_Transform_Cache) para entender lo que ocurre realmente)

Esto es útil porque permite dibujar quads (conjuntos de dos triángulos, es decir en 6 vértices) tan solo usando los datos de un VBO para cuatro vértices

esta tecnica es conocida como **index drawing** y se considera una optimización importante a la hora de hacer computer graphics.

# About Opengl Context and VBO

GLFW is a helper library that will start the OpenGL "context" for us so that it talks to (almost) any operating system in the same way. The context is a running copy of OpenGL, tied to a window on the operating system. 

We will copy this chunk of memory onto the graphics card in a unit called a vertex buffer object (VBO). To do this we "generate" an empty buffer, set it as the current buffer in OpenGL's state machine by "binding", then copy the points into the currently bound buffer.

# Code buffer and objects review in triangle and quad drawing exercises

**EBO:** Element Buffer Object, is an additional buffer for make possible `index drawing`. This drawing technique allow us to take adventage of the fancy `Post Transform Cache` this mean that if the system can detect that you have passed the same vertex attribute inputs, then the system can avoid executing the vertex shader again. Instead, if the outputs for that input attribute set is in the cache, the cached data can be used. This saves vertex processing.  read more [here](https://www.opengl.org/discussion_boards/showthread.php/199688-Element-Buffer-Objects) and [here](https://www.khronos.org/opengl/wiki/Post_Transform_Cache)

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

**VBO:** This buffer is generated for allocate memory in GPU from CPU, the buffer type of a Vertex Buffer Object is GL_ARRAY_BUFFER

	// generate VBO for allocate memory in GPU
	unsigned int VBO;
	glGenBuffers(1, VBO);
	// bind VBO with GL_ARRAY_BUFFER target
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// copy data from CPU to GPU
	//		use the currently bounded buffer to GL_ARRAY_BUFFER as container
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

**Tell OpenGL how it should interpret the vertex data:**

		// tell OpenGL how it should interpret the vertex data(per
	// vertex attribute) using glVertexAttribPointer:
	// glVertexAttribPointer(index = [vertex attrib location remember the layout (location = n) keyword in vertex shader], 
	//						size = [is vec2 = 2, vec3 = 3, etc..],
	//						type = [GL_FLOAT, GL_BOOL, etc..], 
	//						normalize = [opengl should normalize the given data?],
	//						stride = [distance between each "position" ternas in VBO],
	//						start = [whare is the start index of "position"?];
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0); // enable the vertex attribute at location 0

**VAO - Vertex Array Object:** A vertex array object stores the following:

- Calls to `glEnableVertexAttribArray` or `glDisableVertexAttribArray`.

- Vertex attribute configurations via `glVertexAttribPointer`.

- Vertex buffer objects associated with vertex attributes by calls to glVertexAttribPointer.

Ejemplo:

	// generate VAO for store status of subsequent "vertex attribute" calls
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); // bind VAO

**Generate vertex/fragment shader objects:**

	// generate vertex shader
	unsigned int vertex_shader;
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	// attach the shader source code to shader object
	glShaderSource(vertex_shader, 1, &vertex_shader_src, NULL);
	glCompileShader(vertex_shader); // compile shader

**Check shader compile-time errors**

	// check for compile-time errors
	int succes;
	char infolog[512];
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &succes);
	if (!succes)
	{
		glGetShaderInfoLog(vertex_shader, sizeof(infolog), NULL, infolog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infolog << "\n";
	}

**Generate shader program object:** A shader program object is the final linked version of multiple shaders combined. 

To use the recently compiled shaders we have to link them to a shader program object and then activate this shader program when rendering objects.

When linking the shaders into a program it links the outputs of each shader to the inputs of the next shader. This is also where you'll get linking errors if your outputs and inputs do not match.

	// generate shader program object
	unsigned int shader_program;
	shader_program = glCreateProgram();
	// attach the previuosly compiled shaders to shader program object
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program); // link shaders

**Check linking errors in shader program**

	// check linking errors
	glGetProgramiv(shader_program, GL_LINK_STATUS, &succes);
	if (!succes)
	{
		glGetProgramInfoLog(shader_program, sizeof(infolog), NULL, infolog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infolog << "\n";
	}

**Basic structure program without VAO:** This is common in OpenGL ES and WebGL applications

	// 0. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 1. then set the vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);  
	// 2. use our shader program when we want to render an object
	glUseProgram(shaderProgram);
	// 3. now draw the object 
	someOpenGLFunctionThatDrawsOurTriangle(); 

**Basic structure program with VAO:**

	// ..:: Initialization code (done once (unless your object frequently changes)) :: ..
	// 1. bind Vertex Array Object
	glBindVertexArray(VAO);
	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);  
	
	  
	[...]
	
	// ..:: Drawing code (in render loop) :: ..
	// 4. draw the object
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	someOpenGLFunctionThatDrawsOurTriangle();

**Draw: **

To draw our objects of choice OpenGL provides us with the `glDrawArrays` function that draws `primitives` using the **currently active shader**, the previously defined **vertex attribute configuration** and with the **VBO's vertex data** (indirectly bound via the VAO).

	glUseProgram(shader_program); // use a shader program
	glBindVertexArray(VAO); // bind object VAO
	glDrawArrays(GL_TRIANGLES, 0, 3); // draw triangle

The `glDrawArrays` function takes as its first argument the OpenGL primitive type we would like to draw. 
The second argument specifies the **starting index** of the **vertex array (`float vertices[]`)** we'd like to draw; we just leave this at 0. 

The last argument specifies how many vertices we want to draw, which is 3 (we only render 1 triangle from our data, which is exactly 3 vertices long).



# About graphic pipeline

The graphics pipeline can be divided into two large parts: the first transforms your 3D coordinates into 2D coordinates and the second part transforms the 2D coordinates into actual colored pixels.

The graphics pipeline takes as input a set of 3D coordinates and transforms these to colored 2D pixels on your screen. 

The graphics pipeline can be divided into several steps where each step requires the output of the previous step as its input. All of these steps are highly specialized (they have one specific function) and can easily be executed in parallel. Because of their parallel nature most graphics cards of today have thousands of small processing cores to quickly process your data within the graphics pipeline by running small programs on the GPU for each step of the pipeline. These small programs are called shaders.

Below you'll find an abstract representation of all the stages of the graphics pipeline. Note that the blue sections represent sections where we can inject our own shaders.

![enter image description here](https://i.imgur.com/q3RBYry.png)

> In order for OpenGL to know what to make of your collection of coordinates and color values OpenGL requires you to hint what kind of render types you want to form with the data. Do we want the data rendered as a collection of points, a collection of triangles or perhaps just one long line? Those hints are called primitives and are given to OpenGL while calling any of the drawing commands. Some of these hints are GL_POINTS, GL_TRIANGLES and GL_LINE_STRIP.

A fragment in OpenGL is all the data required for OpenGL to render a single pixel.

# About vertex data

With the vertex data defined we'd like to send it as input to the first process of the graphics pipeline: the vertex shader. This is done by creating memory on the GPU where we store the vertex data, configure how OpenGL should interpret the memory and specify how to send the data to the graphics card. The vertex shader then processes as much vertices as we tell it to from its memory.

We manage this memory via so called vertex buffer objects (VBO) that can store a large number of vertices in the GPU's memory. The advantage of using those buffer objects is that we can send large batches of data all at once to the graphics card without having to send data a vertex a time. Sending data to the graphics card from the CPU is relatively slow, so wherever we can we try to send as much data as possible at once. Once the data is in the graphics card's memory the vertex shader has almost instant access to the vertices making it extremely fast

# About Vertex Attribute Objects (VAO)

And that is it! Everything we did the last few million pages led up to this moment, a VAO that stores our vertex attribute configuration and which VBO to use. Usually when you have multiple objects you want to draw, you first generate/configure all the VAOs (and thus the required VBO and attribute pointers) and store those for later use. The moment we want to draw one of our objects, we take the corresponding VAO, bind it, then draw the object and unbind the VAO again.

# todo
Understand the w component of vectors in GLSL and its relationship with something called perspective division. 

> Learn more in [LearnOpenGL site](https://learnopengl.com/#!Getting-started/Hello-Triangle)