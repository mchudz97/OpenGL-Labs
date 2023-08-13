#pragma once
class CollidingMesh 
{
private:
	Mesh mesh;
	CylidricalCollider collider;
	glm::mat4 transform;
public:
	CollidingMesh(Mesh& mesh, glm::vec3 origin = glm::vec3(0)) 
	{
		this->mesh = mesh;
		glm::vec2 ranges = mesh.getMaxRanges();
		this->collider = CylidricalCollider(origin - glm::vec3(0, .2f, 0), ranges.x, ranges.y + .2f);
		this->transform = glm::translate(glm::mat4(1.f), origin);
		this->mesh.translate(origin);
	}
	bool isColliding(glm::vec3 pos) 
	{
		if (this->collider.isColliding(pos))
			return true;
		return false;
	}
	void render(Shader& s) 
	{
		this->mesh.render(s);
	}
	void shadowRender(Shader& s) 
	{
		this->mesh.shadowRender(s);
	}
	void renderToMap(Shader& s) 
	{
		this->mesh.renderToMap(s);
	}
	
};