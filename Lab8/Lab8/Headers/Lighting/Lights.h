#pragma once
struct PointLight
{
	glm::vec3 position;

	glm::vec3 color;

	float constant;
	float linear;
	float quad;

	bool isStatic;
};

