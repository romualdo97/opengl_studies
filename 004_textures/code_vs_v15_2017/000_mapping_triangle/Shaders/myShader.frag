#version 330 core

// FRAGMENT SHADER INPUT
// from vertex shader
in vec3 vColor;
in vec2 vTexCoord;

// from CPU as uniform
uniform float uTime;
uniform sampler2D uTexture;

// FRAGMENT SHADER OUTPUT
out vec4 FragColor;

// LOCAL IDENTIFIERS
#define PI 3.141516

void main()
{
	// The texel of this fragment shader 
	// is then the (filtered) color of the 
	// texture at the (interpolated) texture coordinate.
	
	vec4 anim = vec4(0.5 + 0.5 * sin(uTime), 0.5 + 0.5 * sin(uTime + PI), 0.5 + 0.5 * sin(uTime + 0.5 * PI), 1.0);
	vec4 texel = texture(uTexture, vTexCoord);
	vec4 tint = vec4(vec3(vColor), 1.0) * anim;
	tint = 0.5 + 0.9 * tint;
	FragColor =  texel * tint;
}