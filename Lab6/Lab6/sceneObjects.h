#pragma once
#define OBJ_AMOUNT 7
#define ST_OBJ_AMOUNT 2
#define EMISSION_OBJ_AMOUNT 1


std::vector<glm::vec3> firePositions = {
	glm::vec3(9.4021, 4.42164, -6.22175),
	glm::vec3(9.4021, 4.42164, 6.22175),
	glm::vec3(-9.4021, 4.42164, -6.22175),
	glm::vec3(-9.4021, 4.42164, 6.22175),
	glm::vec3(6.33911, 4.42164, -9.36901),
	glm::vec3(6.33911, 4.42164, 9.36901),
	glm::vec3(-6.33911, 4.42164, 9.36901),
	glm::vec3(-6.33911, 4.42164, -9.36901),
	glm::vec3(3.60202, 6.97752, -2.14058),
	glm::vec3(3.60202, 6.97752, 2.14058),
	glm::vec3(-3.60202, 6.97752, -2.14058),
	glm::vec3(-3.60202, 6.97752, 2.14058),
	glm::vec3(5.90828, 0.551271, -4.3801),
	glm::vec3(5.90828, 0.551271, 4.3801),
	glm::vec3(9.77278, 4.42164, -1.84826),
	glm::vec3(9.77278, 4.42164, 1.84826)

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
	"Objects/castle2.obj",
	"Objects/castle_floor.obj",
	"Objects/castle_roof.obj",
	"Objects/ground.obj",
	"Objects/doors.obj",
	"Objects/walls.obj",
	"Objects/road.obj"
};
const char* texNames[OBJ_AMOUNT * 3] = {
	"Textures/Wall2/diffuse.jpg",
	"Textures/Wall2/diffuse.jpg",
	"Textures/Wall2/normal2.jpg",
	"Textures/Wood/diffuse.jpg",
	"Textures/Wood/specular.png",
	"Textures/Wood/normal.jpg",
	"Textures/Roof/diffuse.jpg",
	"Textures/Roof/specular.png",
	"Textures/Roof/normal.jpg",
	"Textures/Ground/diffuse.png",
	"Textures/Ground/specular.png",
	"Textures/Ground/normal.png",
	"Textures/Door/diffuse.jpg",
	"Textures/Door/specular.png",
	"Textures/Door/normal.jpg",
	"Textures/Wall2/diffuse.jpg",
	"Textures/Wall2/diffuse.jpg",
	"Textures/Wall2/normal2.jpg",
	"Textures/Road/diffuse.jpg",
	"Textures/Road/specular.jpg",
	"Textures/Road/normal.jpg"
};
const char* emissionObjNames[EMISSION_OBJ_AMOUNT] = {
	"Objects/sprite.obj"
};
const char* emissionTexNames[EMISSION_OBJ_AMOUNT] = {
	"Textures/sprite.png"
};

const char* staticObjNames[ST_OBJ_AMOUNT] = {
	"Objects/vessel.obj",
	"Objects/sticks.obj"
};
const char* staticTexNames[ST_OBJ_AMOUNT * 3] = {
	"Textures/Vessel/diffuse.png",
	"Textures/Vessel/diffuse.png",
	"Textures/Vessel/normal.png",
	"Textures/Sticks/diffuse.jpg",
	"Textures/Sticks/specular.png",
	"Textures/Sticks/normal.jpg"
};
void setDirectionLight(GLuint idProgram, bool disabled = false) {
	
	glm::vec3 direction = glm::vec3(-5.f, 10.0f, 20.f);
	glm::vec3 ambient = glm::vec3(.1f);
	glm::vec3 diffuse = glm::vec3(1.f, 1.f, 0.f);
	glm::vec3 specular = glm::vec3(1.f, 1.f, 0.f);
	if (disabled) 
	{
		diffuse = glm::vec3(0.f);
		specular = glm::vec3(0.f);
	}
	glUniform3fv(glGetUniformLocation(idProgram, "dirLight.direction"), 1, glm::value_ptr(direction));
	glUniform3fv(glGetUniformLocation(idProgram, "dirLight.ambient"), 1, glm::value_ptr(ambient));
	glUniform3fv(glGetUniformLocation(idProgram, "dirLight.diffuse"), 1, glm::value_ptr(diffuse));
	glUniform3fv(glGetUniformLocation(idProgram, "dirLight.specular"), 1, glm::value_ptr(specular));
}
void setFireUniforms(GLuint idProgram, int currFrame, int maxFrame, bool disabled = false)
{
	float str = cos(((float)currFrame - float(maxFrame) / 2) * 2 / maxFrame);
	glm::vec3 amb = glm::vec3(.0f, .0f, .0f) * str;
	glm::vec3 diff = glm::vec3(1.f, .5f, .0f) * str;
	glm::vec3 spec = glm::vec3(1.f, .5f, .0f) * str;
	float constant = 1.f;
	float linear = 0.14f;
	float quad = .014f;
	if (disabled) 
	{
		diff = glm::vec3(0.f);
		spec = glm::vec3(0.f);
	}
	for (GLuint i = 0; i < firePositions.size(); i++)
	{
		string number = to_string(i);

		glUniform3fv(glGetUniformLocation(idProgram, ("pointLights[" + number + "].position").c_str()), 1, glm::value_ptr(firePositions[i]));
		glUniform3fv(glGetUniformLocation(idProgram, ("pointLights[" + number + "].ambient").c_str()), 1, &amb[0]);
		glUniform3fv(glGetUniformLocation(idProgram, ("pointLights[" + number + "].diffuse").c_str()), 1, &diff[0]);
		glUniform3fv(glGetUniformLocation(idProgram, ("pointLights[" + number + "].specular").c_str()), 1, &spec[0]);
		glUniform1f(glGetUniformLocation(idProgram, ("pointLights[" + number + "].constant").c_str()), constant);
		glUniform1f(glGetUniformLocation(idProgram, ("pointLights[" + number + "].linear").c_str()), linear);
		glUniform1f(glGetUniformLocation(idProgram, ("pointLights[" + number + "].quadratic").c_str()), quad);
	}

}