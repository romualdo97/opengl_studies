#version 330 core

// Surface that can receive light from a directional light

// Keep in mind that sampler2D is a so called opaque type which means we can't
// instantiate these types, but only define them as uniforms. If the struct
// would be instantiated other than as a uniform (like a function parameter)
// GLSL could throw strange errors; the same thus applies to any struct holding such opaque types.

// Surface properties
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

// Light properties
struct Light {
    // vec3 position; // no longer necessary when using directional lights.
	vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// FRAGMENT SHADER INPUT
// from vertex shader
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
    vec3 ambient = uLight.ambient * texture(uMaterial.diffuse, vTexCoord).rgb;
	
	// calculate diffuse
	vec3 norm = normalize(vNormal);
	vec3 lightDir = normalize(-uLight.direction); // normalize(uLight.position - vFragPos);  
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = uLight.diffuse * (diff * texture(uMaterial.diffuse, vTexCoord).rgb);

	// Calculate specular
	vec3 viewDir = normalize(uViewPos - vFragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);
	vec3 specular = uLight.specular * (spec * texture(uMaterial.specular, vTexCoord).rgb);  

	// Output color
	vec3 result = ambient + diffuse;// + specular;
	FragColor = vec4(result, 1.0);
}