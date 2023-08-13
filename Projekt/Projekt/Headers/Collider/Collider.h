#pragma once
class Collider
{
private:
	glm::mat4 transform = glm::mat4(1.f);
public:
	virtual bool isColliding(glm::vec3 point) {
		return false;
	};

	void setTransform(glm::mat4 transform) 
	{
		this->transform = transform;
	}
	glm::mat4 getTransform() 
	{
		return this->transform;
	}
};