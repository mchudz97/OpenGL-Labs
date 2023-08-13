#pragma once
#define OBJ_AMOUNT 7
std::vector<glm::vec3> lampPositions = {
	glm::vec3(-3.2, 0.35, 1.43),
	glm::vec3(4.38, 0.35, 1.43)
};

std::vector<glm::vec3> firePositions = {
	glm::vec3(-3.2, 4, 1.43),
	glm::vec3(4.38, 4, 1.43),
	glm::vec3(-1, 2, 2.43),
	glm::vec3(1.01, 2, 2.43)
};
std::vector<glm::vec3> lightPositions = {
	glm::vec3(-3.2, 2.95, 1.43),
	glm::vec3(4.38, 2.95, 1.43)
};

std::vector<glm::vec3> createLineOfPositions(glm::vec3 origin, float length, float distance, glm::vec3 direction) 
{
	vector<glm::vec3> positions;
	glm::vec3 actualPos = origin;
	positions.push_back(actualPos);
	int amount = length / distance;
	for (int i = 0; i < amount; i++) {
		actualPos += direction * distance;
		positions.push_back(actualPos);
	}
	return positions;
}
const char* objNames[OBJ_AMOUNT] = {
	"Objects/Ground.obj",
	"Objects/Lamp.obj",
	"Objects/Glass.obj",
	"Objects/Road.obj",
	"Objects/Bench_armature.obj",
	"Objects/Bench_boards.obj",
	"Objects/Sprite.obj"
};
const char* texNames[OBJ_AMOUNT] = {
	"Textures/Ground.jpg",
	"Textures/Lamp.jpg",
	"Textures/Glass.jpg",
	"Textures/Road.jpg",
	"Textures/Bench_armature.jpg",
	"Textures/Bench_boards.jpg",
	"Textures/Sprite.png"
};
void setLightUniforms(GLuint idProgram) 
{
	glm::vec3 amb = glm::vec3(.8f);
	glm::vec3 diff = glm::vec3(.8f);
	glm::vec3 spec = glm::vec3(.6f);
	float constant = 1.f;
	float linear = 0.7f;
	float quad = 0.01f;
	glm::vec3 direction = glm::vec3(-1.f, -1.0f, 2.f);
	glm::vec3 dirAmb = glm::vec3(.1f);
	glm::vec3 dirDiff = glm::vec3(.1f);
	glm::vec3 dirSpec = glm::vec3(.1f);
	glUniform3fv(glGetUniformLocation(idProgram, "dirLight.direction"), 1, glm::value_ptr(direction));
	glUniform3fv(glGetUniformLocation(idProgram, "dirLight.ambient"), 1, glm::value_ptr(dirAmb));
	glUniform3fv(glGetUniformLocation(idProgram, "dirLight.diffuse"), 1, glm::value_ptr(dirDiff));
	glUniform3fv(glGetUniformLocation(idProgram, "dirLight.specular"), 1, glm::value_ptr(dirSpec));

	for (GLuint i = 0; i < lightPositions.size(); i++)
	{
		string number = to_string(i);

		glUniform3fv(glGetUniformLocation(idProgram, ("pointLights[" + number + "].position").c_str()), 1, glm::value_ptr(lightPositions[i]));
		glUniform3fv(glGetUniformLocation(idProgram, ("pointLights[" + number + "].ambient").c_str()), 1, &amb[0]);
		glUniform3fv(glGetUniformLocation(idProgram, ("pointLights[" + number + "].lightColor").c_str()), 1, glm::value_ptr(glm::vec3(.9)));
		glUniform3fv(glGetUniformLocation(idProgram, ("pointLights[" + number + "].diffuse").c_str()), 1, &diff[0]);
		glUniform3fv(glGetUniformLocation(idProgram, ("pointLights[" + number + "].specular").c_str()), 1, &spec[0]);
		glUniform1f(glGetUniformLocation(idProgram, ("pointLights[" + number + "].constant").c_str()), constant);
		glUniform1f(glGetUniformLocation(idProgram, ("pointLights[" + number + "].linear").c_str()), linear);
		glUniform1f(glGetUniformLocation(idProgram, ("pointLights[" + number + "].quadratic").c_str()), quad);
	}

}