#version 330 core

// Custom definitions
struct Material
{
	sampler2D diffuseTexture1;
	sampler2D textureSpecular1;
};

// FRAGMENT SHADER INPUT
// from vertex shader
in vec3 vNormal;
in vec2 vTexCoord;

// from CPU as uniform
uniform float uTime;
uniform Material uMaterial;

// FRAGMENT SHADER OUTPUT
out vec4 FragColor;

void main()
{
	FragColor = texture(uMaterial.diffuseTexture1, vTexCoord); //vec4(vTexCoord, 0.0, 1.0);
}