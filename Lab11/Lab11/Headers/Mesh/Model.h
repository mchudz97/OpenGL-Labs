#pragma once
class Model 
{
private:
	vector<Mesh> meshes;
	CFont nameplate;
	glm::mat4 transform = glm::mat4(1.f);
public:
	Model(){}
	
	void attachMesh(Mesh &m) 
	{
		this->meshes.push_back(m);
	}

	void attachMultipleMeshes(vector<Mesh> meshes) {
		this->meshes.insert(this->meshes.end(), meshes.begin(), meshes.end());
	}

	void translate(glm::vec3 vec) 
	{
		this->transform = glm::translate(this->transform, vec);
	}
	//need to update in the future
};