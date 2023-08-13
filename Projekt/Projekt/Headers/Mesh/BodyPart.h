#pragma once
#include <cstddef>

class BodyPart 
{
private:
	Mesh* bodyPartMesh;
	vector<BodyPart*> subParts;

public:
	BodyPart(Mesh* part, BodyPart *parent = NULL) 
	{
		this->bodyPartMesh = part;
		if(parent)
			parent->attachPart(this);
	}
	void attachPart(BodyPart* part) 
	{
		this->subParts.push_back(part);
	}
	void render(Shader& shader, glm::mat4 parentalTransform = glm::mat4(1)) 
	{
		glm::mat4 transform = parentalTransform * this->bodyPartMesh->getTransform();
		this->bodyPartMesh->renderStacked(shader, transform);
		for (BodyPart* bp : subParts) {
			glm::mat4 tr = glm::mat4(transform);
			bp->render(shader, tr);
		}
	}

	void shadowRender(Shader& shader, glm::mat4 parentalTransform = glm::mat4(1))
	{
		glm::mat4 transform = parentalTransform * this->bodyPartMesh->getTransform();
		this->bodyPartMesh->shadowRenderStacked(shader, transform);
		for (BodyPart* bp : subParts) {
			glm::mat4 tr = glm::mat4(transform);
			bp->shadowRender(shader, tr);
		}
	}

	void translate(glm::vec3 to) 
	{
		this->bodyPartMesh->translate(to);
	}
	void rotate(glm::vec3 xyz, GLfloat angle) 
	{
		this->bodyPartMesh->rotate(xyz, angle);
	}
};