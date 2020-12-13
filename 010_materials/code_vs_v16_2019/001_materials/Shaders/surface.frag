#version 330 core

// Surface properties
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

// Light properties
struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// FRAGMENT SHADER INPUT
// from vertex shader
in vec3 vColor;
in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vFragPos;

// from CPU as uniform
uniform Material uMaterial;
uniform Light uLight;
uniform vec3 uViewPos;

// FRAGMENT SHADER OUTPUT
out vec4 FragColor;

void main()
{
	// calculate ambient color
    vec3 ambient = uLight.ambient * uMaterial.ambient;
	
	// calculate diffuse
	vec3 norm = normalize(vNormal);
	vec3 lightDir = normalize(uLight.position - vFragPos);  
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = uLight.diffuse * (diff * uMaterial.diffuse);

	// Calculate specular
	vec3 viewDir = normalize(uViewPos - vFragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);
	vec3 specular = uLight.specular * (spec * uMaterial.specular);  

	// Output color
	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}