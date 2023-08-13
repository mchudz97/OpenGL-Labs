#pragma once
class CylidricalCollider 
{
private:
	glm::vec3 pos;
	GLfloat r;
	GLfloat h;
public:
	CylidricalCollider(){}
	CylidricalCollider(glm::vec3 origin, GLfloat r, GLfloat h) 
	{
		this->pos = origin;
		this->r = r;
		this->h = h;
	}
	bool isColliding(glm::vec3 pos) 
	{
		GLfloat dist = glm::distance(pos, this->pos);
		if (dist > this->r)
			return false;
		if(pos.y > this->pos.y + h || pos.y < this->pos.y)
			return false;
		return true;
	}

};