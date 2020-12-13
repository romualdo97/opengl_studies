#version 330 core

// FRAGMENT SHADER INPUT
// from vertex shader
in vec3 vColor;
in vec2 vTexCoord;

// FRAGMENT SHADER OUTPUT
out vec4 FragColor;

// from CPU as uniform
uniform sampler2D uScreenTexture;
uniform float uTime;

void main()
{	
	vec4 texel = texture(uScreenTexture, vTexCoord);

	float t = 0.5 * sin(uTime) + 0.5;

	vec4 finalColor = mix(vec4(texel.r), texel, t);

	FragColor = finalColor;
}