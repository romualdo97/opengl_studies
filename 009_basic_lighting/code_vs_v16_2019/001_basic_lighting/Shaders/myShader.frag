#version 330 core

// FRAGMENT SHADER INPUT
// from vertex shader
in vec3 vColor;
in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vFragPos;

// from CPU as uniform
uniform vec3 uObjectColor;
uniform vec3 uLightColor;
uniform vec3 uLightPos;
uniform vec3 uViewPos;

// FRAGMENT SHADER OUTPUT
out vec4 FragColor;

// LOCAL IDENTIFIERS
#define PI 3.141516
vec4 color;

// use the vertex colors?
// #define SWAP_TEXTURES

void main()
{
	// calculate ambient color
	float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * uLightColor;
	
	// calculate diffuse
	vec3 norm = normalize(vNormal);
	vec3 lightDir = normalize(uLightPos - vFragPos);  
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * uLightColor;

	// Calculate specular
	float specularStrength = 0.5;
	vec3 viewDir = normalize(uViewPos - vFragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * uLightColor;  

	vec3 result = (ambient + diffuse + specular) * uObjectColor;
	FragColor = vec4(result, 1.0);
}