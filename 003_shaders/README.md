# `Shader` class specification
Was built a class for create an extra abstraction layer for compile vertex and fragment shader as well as to link them into shader programs.

**Constructors**

    // Constructor: Specify vertex and fragment shader path
	// Shader("myVert.vert", "MyFrag.frag");
	Shader::Shader(GLchar const *vertexPath, GLchar const *fragmentPath);

Specify a path to an ASCII text file with `*.vert` and `*.frag` extension for the vertex and fragment shader source code respectively.

---

	// Constructor: Specify just a shader name (no shader file extension) this constructor assumes the vertex and fragment shader are named equally with its respective different file extensions, 
	// e.g: Shader("MyShader"); assumes two files; MyShader.frag, MyShader.vert
	Shader::Shader(GLchar const *shaderName);

Specify a path to a equally named `vertex` and `fragment` shader with its respective file extensions `*.vert` and `*.frag`

**Use shader program**

	// use shader program
	void Shader::use();

# About `vertex shaders`

When we're talking specifically about the `vertex shader` each input variable is also known as a `vertex attribute`. There is a **maximum number** of `vertex attributes` we're allowed to declare limited by the hardware. OpenGL guarantees **there are always at least 16 4-component** `vertex attributes` available, but some hardware might allow for more which you can retrieve by querying `GL_MAX_VERTEX_ATTRIBS`:

When passing more than one vertex attribute the `glVertexAttribPointer` function must specify the new VBO structure.

![enter image description here](https://i.imgur.com/G1O6fqP.png)

# GLSL swizzling

	vec2 someVec;
	vec4 differentVec = someVec.xyxx;
	vec3 anotherVec = differentVec.zyw;
	vec4 otherVec = someVec.xxxx + anotherVec.yxzy;

# Uniforms

Uniforms are another way to pass data from our application on the CPU to the shaders on the GPU, but uniforms are slightly different compared to vertex attributes. First of all, uniforms are global. Global, meaning that a uniform variable is unique per shader program object, and can be accessed from any shader at any stage in the shader program. Second, whatever you set the uniform value to, uniforms will keep their values until they're either reset or updated.

> If you declare a uniform that isn't used anywhere in your GLSL code the compiler will silently remove the variable from the compiled version which is the cause for several frustrating errors; keep this in mind!

**Pass integers:**

	unsigned int uniformLoc = glGetUniformLocation(ID, "uName");
	glUniform1i(uniformLoc, value);

**Pass bools:**

	unsigned int uniformLoc = glGetUniformLocation(ID, "uName");
	glUniform1i(uniformLoc, value);

**Pass floats:**

	unsigned int uniformLoc = glGetUniformLocation(ID, "uName");
	glUniform1f(uniformLoc, value);

**Pass a GLSL vec3:**
		
	unsigned int uniformLoc = glGetUniformLocation(ID, "uName");
	glUniform4f(uniformLoc, value);
	
> Learn moret at [StackEdit](https://learnopengl.com/#!Getting-started/Shaders).