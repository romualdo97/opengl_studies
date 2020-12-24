#version 330 core

// Surface that can receive light from
// - 1 directional light
// - 3 point light
// - 2 spot lights

#define NR_POINT_LIGHTS 3
#define NR_SPOT_LIGHTS 2

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

// Surface properties unpacked (get texels directly)
struct UnpackedMaterial
{
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

// Directional light properties
struct DirectionalLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Point light properties
struct PointLight
{
	vec3 position;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constant;
    float linear;
    float quadratic;
};

struct SpotLight
{
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
uniform Material uMaterial; // Material of this surface
uniform vec3 uViewPos; // Position of camera in world space
uniform DirectionalLight uDirectionalLight; // Directional light info
uniform PointLight uPointLights[NR_POINT_LIGHTS]; // Point light info
uniform SpotLight uSpotLights[NR_SPOT_LIGHTS]; // Spot light info

// FRAGMENT SHADER OUTPUT
out vec4 FragColor;

// FUNCTION FORWARD DECLARATIONS
vec3 calculateDirectionalLight(vec3 cameraPosition, DirectionalLight light, UnpackedMaterial material);
vec3 calculatePointLight(vec3 cameraPosition, PointLight light, UnpackedMaterial material);
vec3 calculateSpotLight(vec3 cameraPosition, SpotLight light, UnpackedMaterial material);

void main()
{
    // Unpack material so it can be used on lighting calculations
    vec3 diffuseTexel = texture(uMaterial.diffuse, vTexCoord).rgb; // Get color of surface from sampler
    vec3 specularTexel = texture(uMaterial.specular, vTexCoord).rgb; // Get specular factors of surface from sampler
    UnpackedMaterial material = UnpackedMaterial(diffuseTexel, specularTexel, uMaterial.shininess); // Use GLSL struct constructor

    // Final color
    vec3 color = vec3(0.0f);

    // Calculate directional light
    color = calculateDirectionalLight(uViewPos, uDirectionalLight, material);

    // Calculate point lights
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        color += calculatePointLight(uViewPos, uPointLights[i], material);
    }

    // Calculate spot lights
    for (int i = 0; i < NR_SPOT_LIGHTS; i++)
    {
        color += calculateSpotLight(uViewPos, uSpotLights[i], material);
    }

	FragColor = vec4(color, 1.0);
}

vec3 calculateDirectionalLight(vec3 cameraPosition, DirectionalLight light, UnpackedMaterial material)
{
    // calculate ambient color
    vec3 ambient = light.ambient * material.diffuse;

    // calculate diffuse
	vec3 norm = normalize(vNormal);
	vec3 lightDir = normalize(-light.direction); // From surface to light
	float diff = max(dot(norm, lightDir), 0.0); // Range [0, 1] based on surface normal and lightDir
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // Calculate specular
	vec3 viewDir = normalize(uViewPos - vFragPos);
	vec3 reflectDir = reflect(light.direction, norm); // Reflection of ray from light to surface
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);

    return ambient + diffuse + specular;
}

vec3 calculatePointLight(vec3 cameraPosition, PointLight light, UnpackedMaterial material)
{
    // Calculate attenuation factor
	// https://www.desmos.com/calculator/wtt73x5kwp
	// https://learnopengl.com/Lighting/Light-casters
	float d = length(light.position - vFragPos);
	float attenuation = 1.0 / (light.constant + light.linear * d + light.quadratic * d * d);

    // calculate ambient color
    vec3 ambient = light.ambient * material.diffuse;

    // calculate diffuse
	vec3 norm = normalize(vNormal);
	vec3 lightDir = normalize(light.position - vFragPos);  
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // Calculate specular
	vec3 viewDir = normalize(uViewPos - vFragPos);
	vec3 reflectDir = reflect(-lightDir, norm); // Reflection of ray from light to surface
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);
	vec3 specular = light.specular * (spec * material.diffuse); 

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

vec3 calculateSpotLight(vec3 cameraPosition, SpotLight light, UnpackedMaterial material)
{
    // Calculate if this shader is inside the spot
	vec3 lightDir = normalize(light.position - vFragPos);
	float cosine = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((cosine - light.outerCutOff) / epsilon, 0.0, 1.0);

    // Store the final color
	vec3 finalColor = vec3(0.0);

    if (cosine > light.outerCutOff) // do lighting calculations
    {
        // calculate ambient color
		vec3 ambient = light.ambient * material.diffuse;
	
		// calculate diffuse
		vec3 norm = normalize(vNormal);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = light.diffuse * (diff * material.diffuse);

        // Calculate specular
		vec3 viewDir = normalize(uViewPos - vFragPos);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);
		vec3 specular = light.specular * (spec * material.diffuse);  

        // Apply smoth edges to spot
		diffuse *= intensity;
		specular *= intensity;

        finalColor = ambient + diffuse + specular;
    }
    else 
    {
        // else, use ambient light so scene isn't completely dark outside the spotlight.
		finalColor = light.ambient * material.diffuse;
    }

    return finalColor;
}