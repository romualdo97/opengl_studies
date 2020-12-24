#version 330 core

// Surface that can receive light from a spot light

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
    vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;
  
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
	// Calculate if this shader is inside the spot
	vec3 lightDir = normalize(uLight.position - vFragPos);
	float cosine = dot(lightDir, normalize(-uLight.direction));
	float epsilon = uLight.cutOff - uLight.outerCutOff;
	float intensity = clamp((cosine - uLight.outerCutOff) / epsilon, 0.0, 1.0);
	
	// Store the final color
	vec3 finalColor = vec3(1.0);

	if (cosine > uLight.outerCutOff) // do lighting calculations
	{
		// calculate ambient color
		vec3 ambient = uLight.ambient * texture(uMaterial.diffuse, vTexCoord).rgb;
	
		// calculate diffuse
		vec3 norm = normalize(vNormal);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = uLight.diffuse * (diff * texture(uMaterial.diffuse, vTexCoord).rgb);

		// Calculate specular
		vec3 viewDir = normalize(uViewPos - vFragPos);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);
		vec3 specular = uLight.specular * (spec * texture(uMaterial.specular, vTexCoord).rgb);  

		// Apply smoth edges to spot
		diffuse *= intensity;
		specular *= intensity;

		// Output color
		finalColor = ambient + diffuse + specular;
	}
	else
	{
		// else, use ambient light so scene isn't completely dark outside the spotlight.
		finalColor = uLight.ambient * texture(uMaterial.diffuse, vTexCoord).rgb;
	}
	
	FragColor = vec4(finalColor, 1.0);
}