#pragma once

glm::vec3 spotLightPositions[NR_SPOT_LIGHTS]
{
	glm::vec3(-1.0f, -1.0f, -1.0f),
	glm::vec3(-1.0f, 1.0f, -6.0f)
};

glm::vec3 spotLightDirections[NR_SPOT_LIGHTS]
{
	glm::vec3(0.0f, 0.0f, -4.0f),
	glm::vec3(0.5f, -1.0f, 0.0f)
};

float spotLightCutOffs[NR_SPOT_LIGHTS]
{
	12.0f, 12.0f
};

float spotLightOuterCutOffs[NR_SPOT_LIGHTS]
{
	15.0f, 15.0f
};

glm::vec3 spotLightColors[NR_SPOT_LIGHTS]
{
	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, 0.0f, 1.0f)
};