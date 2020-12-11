#version 330 core

// FRAGMENT SHADER INPUT
// from vertex shader
in vec3 vColor;
in vec2 vTexCoord;

// FRAGMENT SHADER OUTPUT
out vec4 FragColor;

// from CPU as uniform
uniform sampler2D uScreenTexture;

void main()
{	
	vec4 texel = texture(uScreenTexture, vTexCoord);
	FragColor = vec4(texel.r);
}