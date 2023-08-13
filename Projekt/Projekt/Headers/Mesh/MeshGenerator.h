#pragma once
Mesh* simpleMeshPtr(const char* path)
{
	std::vector<Vertex> verts;
	std::vector<GLuint> idxs;
	loadOBJandSetIndices(path, verts, idxs);
	return new Mesh(verts, idxs);
}

Mesh* createTexturedMesh(
	const char* path,
	TextureManager& tm,
	bool Anisotropy,
	const char* defaultPath = NULL,
	const char* albedoPath = NULL,
	const char* roughnessPath = NULL,
	const char* aoPath = NULL,
	const char* metallicPath = NULL,
	const char* normalPath = NULL,
	int frames = 0) 
{
	std::vector<Vertex> verts;
	std::vector<GLuint> idxs;
	loadOBJandSetIndices(path, verts, idxs);

	return new Mesh(
		verts, 
		idxs,
		tm.getTexture(defaultPath, Anisotropy),
		frames,
		tm.getTexture(albedoPath, Anisotropy),
		tm.getTexture(roughnessPath, Anisotropy),
		tm.getTexture(aoPath, Anisotropy),
		tm.getTexture(metallicPath, Anisotropy),
		tm.getTexture(normalPath, Anisotropy)
	);
}


void generateInstancedMesh(
	const char* path,
	std::vector<InstantableMesh>& container,
	TextureManager& tm,
	bool Anisotropy,
	std::vector<glm::vec3> positions,
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

	std::vector<glm::mat4> matrices;
	for (int i = 0; i < positions.size(); i++) 
	{
		matrices.push_back(glm::translate(glm::mat4(1.f), positions[i]));
	}
	container.push_back(
		InstantableMesh(
			verts,
			idxs,
			matrices,
			tm.getTexture(defaultPath, Anisotropy),
			frames,
			tm.getTexture(albedoPath, Anisotropy),
			tm.getTexture(roughnessPath, Anisotropy),
			tm.getTexture(aoPath, Anisotropy),
			tm.getTexture(metallicPath, Anisotropy),
			tm.getTexture(normalPath, Anisotropy)
			)
	);
}
void generateNamedMesh(
	CFont &c,
	float textHeight,
	string name,
	const char* path,
	std::vector<NamedMesh>& container,
	TextureManager& tm,
	bool Anisotropy,
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

	container.push_back(
		NamedMesh(
			c,
			textHeight,
			name,
			verts,
			idxs,
			tm.getTexture(defaultPath, Anisotropy),
			frames,
			tm.getTexture(albedoPath, Anisotropy),
			tm.getTexture(roughnessPath, Anisotropy),
			tm.getTexture(aoPath, Anisotropy),
			tm.getTexture(metallicPath, Anisotropy),
			tm.getTexture(normalPath, Anisotropy)
			)
		);
}
void generateMeshes(
	const char* path,
	std::vector<Mesh> &container,
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
			container.push_back(std::move(m));

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
		container.push_back(std::move(m));
	}
}

