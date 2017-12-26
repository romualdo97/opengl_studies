#version 330 core

// VERTEX SHADER INPUT: vertex attributes
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

// VERTEX SHADER OUTPUT: pass info from 'vs' to 'fs'
out vec3 vColor;
out vec2 vTexCoord;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	vColor = aColor;
	vTexCoord = aTexCoord;
}