#pragma once
void generateMeshes(
	const char* path,
	std::vector<Mesh>* container,
	std::vector<glm::vec3>* positions = NULL,
	const char* defaultPath = NULL,
	const char* diffusePath = NULL,
	const char* specularPath = NULL,
	const char* normalPath = NULL,
	bool tangents = false,
	bool removesBlack = false,
	int frames = 0
)
{
	if (positions)
	{
		std::vector<Vertex> verts;
		std::vector<GLuint> idxs;
		loadOBJandSetIndices(path, verts, idxs, tangents);
		for (int i = 0; i < positions->size(); i++)
		{

			Mesh m = Mesh(&verts, &idxs, defaultPath, frames, diffusePath, specularPath, normalPath, removesBlack);
			m.translate((*positions)[i]);
			container->push_back(std::move(m));

		}
	}
	else
	{
		std::vector<Vertex> verts;
		std::vector<GLuint> idxs;
		loadOBJandSetIndices(path, verts, idxs, tangents);

		Mesh m = Mesh(&verts, &idxs, defaultPath, frames, diffusePath, specularPath, normalPath, removesBlack);
		container->push_back(std::move(m));

	}
}