#pragma once
void generateMeshes(
	const char* path,
	std::vector<Mesh>* container,
	std::vector<glm::vec3>* positions = NULL,
	const char* texName = NULL,
	int frames = 0,
	bool removeBlack = false
	) 
{
	if (positions) 
	{
		std::vector<Vertex> verts;
		std::vector<GLuint> idxs;
		loadOBJandSetIndices(path, verts, idxs);
		for (int i = 0; i < positions->size(); i++) 
		{
			
			Mesh m = Mesh(&verts, &idxs, texName, frames, removeBlack);
			m.translate((*positions)[i]);
			container->push_back(std::move(m));
			
		}
	}
	else 
	{
		std::vector<Vertex> verts;
		std::vector<GLuint> idxs;
		loadOBJandSetIndices(path, verts, idxs);

		Mesh m = Mesh(&verts, &idxs, texName, frames, removeBlack);
		container->push_back(std::move(m));
		
	}
}