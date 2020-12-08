#version 330 core

// VERTEX SHADER INPUT: 

// vertex attributes
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;

// uniforms
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

// VERTEX SHADER OUTPUT: pass info from 'vs' to 'fs'
out vec3 vColor;
out vec2 vTexCoord;
out vec3 vNormal;
out vec3 vFragPos;

// LOCAL IDENTIFIERS
vec4 pos;

void main()
{
	gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);
	vColor = aColor;
	vTexCoord = aTexCoord;
	vFragPos = vec3(uModel * vec4(aPos, 1.0));

	// Calculate the normal matrix
	// You need to use a different matrix to transform your normal vectors when your model's transformation might include non-uniform scale.
	// When we stretch a shape along, say, just the y axis, the y coordinate of all of the vertices gets exaggerated, making the object taller.
	// https://gamedev.stackexchange.com/questions/168407/normal-matrix-in-plain-english
	// vNormal = mat3(transpose(inverse(uModel))) * aNormal;

	// You can simply multyply normal by model matrix if you are sure your object is doing an uniform scale
	// In the diffuse lighting section the lighting was fine because we didn't do any scaling on the object, so there was not really a need to use a normal matrix and we could've just multiplied the normals with the model matrix. If you are doing a non-uniform scale however, it is essential that you multiply your normal vectors with the normal matrix.
	// https://learnopengl.com/Lighting/Basic-Lighting
	vNormal = mat3(uModel) * aNormal;
}