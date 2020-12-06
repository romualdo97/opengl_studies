#version 330 core

// FRAGMENT SHADER INPUT
// from vertex shader
in vec3 vColor;
in vec2 vTexCoord;

// from CPU as uniform
uniform vec3 uObjectColor;
uniform vec3 uLightColor;

// FRAGMENT SHADER OUTPUT
out vec4 FragColor;

// LOCAL IDENTIFIERS
#define PI 3.141516
vec4 color;

// use the vertex colors?
// #define SWAP_TEXTURES

void main()
{
	// The texelA, B of this fragment shader 
	// is then the (filtered) color of the 
	// texture at the (interpolated) texture coordinate.
	
	FragColor = vec4(uLightColor * uObjectColor, 1.0);
}