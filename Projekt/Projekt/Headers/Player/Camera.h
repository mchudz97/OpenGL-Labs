#pragma once

class Camera 
{
private:
	glm::mat4 viewTransform = glm::mat4(1.0f);
	GLfloat dist;
	GLfloat horizontal_dist;
	GLfloat vertical_dist;
public:
	Camera(){}
	Camera(GLfloat d)
	{
		this->dist = d;
		this->horizontal_dist = dist * cos(3.14159265359f / 6.f);
		this->vertical_dist = dist * sin(3.14159265359 / 6.f);
	}

	void calculateView(glm::vec3 playerPos, GLfloat playerRotation) 
	{

		GLfloat offset_x = this->horizontal_dist * sin(playerRotation);
		GLfloat offset_z = this->horizontal_dist * cos(playerRotation);
		glm::vec3 cameraPos = glm::vec3(playerPos.x - offset_x, playerPos.y + this->vertical_dist, playerPos.z - offset_z);
		
		GLfloat camRot =  playerRotation;

		this->viewTransform = glm::translate(glm::mat4(1.f), cameraPos);
	}

	glm::mat4 getView() 
	{
		return this->viewTransform;
	}
};