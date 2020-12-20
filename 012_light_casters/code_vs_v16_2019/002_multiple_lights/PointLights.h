#pragma once

glm::vec3 pointLightPositions[NR_POINT_LIGHTS]
{
	glm::vec3(-1.0f, -2.0f, -2.0f),
	glm::vec3(1.0f, -1.0f, -3.0f),
	glm::vec3(0.0, -1.0, -4.0)
};

glm::vec3 pointLightColors[NR_POINT_LIGHTS]
{
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 1.0f)
};

float pointLightConstants[NR_POINT_LIGHTS]
{
	1.0f, 1.0f, 1.0f
};

float pointLightLinears[NR_POINT_LIGHTS]
{
	0.7f, 0.7f, 0.7f
};

float pointLightQuadratics[NR_POINT_LIGHTS]
{
	1.8f, 1.8f, 1.8f
};
