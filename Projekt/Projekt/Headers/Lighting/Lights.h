#pragma once
struct PointLight
{
	glm::vec3 position;

	glm::vec3 color;

	float str;

	float constant;
	float linear;
	float quadratic;

	bool isStatic;
};
struct DirLight 
{
	glm::vec3 position; // its also direction
	glm::vec3 color;
};

