#pragma once
class RectCollider : public Collider 
{
private:
	glm::vec3 pointBL;
	glm::vec3 pointUR;

public:
	RectCollider(glm::vec3 p1, glm::vec3 p2) {
		this->pointBL = p1;
		this->pointUR = p2;
	}
	bool isColliding(glm::vec3 point) {

	}
};