#pragma once

Mesh* simpleMeshPtr(const char* path) 
{
	std::vector<Vertex> verts;
	std::vector<GLuint> idxs;
	loadOBJandSetIndices(path, verts, idxs);
	return new Mesh(verts, idxs);
}

void generateMeshes(
	const char* path,
	std::vector<Mesh>* container,
	TextureManager &tm,
	bool Anisotropy,
	std::vector<glm::vec3>* positions = NULL,
	const char* defaultPath = NULL,
	const char* albedoPath = NULL,
	const char* roughnessPath = NULL,
	const char* aoPath = NULL,
	const char* metallicPath = NULL,
	const char* normalPath = NULL,
	int frames = 0
)
{

	std::vector<Vertex> verts;
	std::vector<GLuint> idxs;
	loadOBJandSetIndices(path, verts, idxs);
	
	if (positions) 
	{
		for (int i = 0; i < positions->size(); i++)
		{

			Mesh m = Mesh(verts,
				idxs,
				tm.getTexture(defaultPath,  Anisotropy),
				frames,
				tm.getTexture(albedoPath,  Anisotropy),
				tm.getTexture(roughnessPath,  Anisotropy),
				tm.getTexture(aoPath,  Anisotropy),
				tm.getTexture(metallicPath,  Anisotropy),
				tm.getTexture(normalPath, Anisotropy)
				);
			m.translate((*positions)[i]);
			container->push_back(std::move(m));

		}
	}
	else 
	{

		Mesh m = Mesh(verts,
			idxs,
			tm.getTexture(defaultPath, Anisotropy),
			frames,
			tm.getTexture(albedoPath, Anisotropy),
			tm.getTexture(roughnessPath, Anisotropy),
			tm.getTexture(aoPath, Anisotropy),
			tm.getTexture(metallicPath, Anisotropy),
			tm.getTexture(normalPath, Anisotropy)
		);
		container->push_back(std::move(m));
	}
}

