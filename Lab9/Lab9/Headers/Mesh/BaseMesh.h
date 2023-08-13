#pragma once
class BaseMesh 
{
private:

	std::vector<Vertex> verts;
	std::vector<GLuint> indices;

	

	GLuint defaultTexture = 0;
	GLuint albedoTexture = 0;
	GLuint roughnessTexture = 0;
	GLuint aoTexture = 0;
	GLuint normalTexture = 0;
	GLuint metallicTexture = 0;


	GLuint maxFrames = 0;
	GLuint actualFrame = 0;


protected:

	GLuint idVAO;
	GLuint idVBO;
	GLuint idVBO_idx;
	GLuint vertsCount;
	GLuint indicesCount;

	void updateTextureUniforms(Shader shader)
	{
		if (this->defaultTexture)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, this->defaultTexture);
			shader.setInt(0, "tex");
		}
		if (this->albedoTexture)
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, this->albedoTexture);
			shader.setInt(1, "material.albedo");
		}
		if (this->roughnessTexture)
		{
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, this->roughnessTexture);
			shader.setInt(2, "material.roughness");
		}
		if (this->aoTexture)
		{
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, this->aoTexture);
			shader.setInt(3, "material.ao");
		}
		if (this->metallicTexture)
		{
			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D, this->metallicTexture);
			shader.setInt(4, "material.metallic");
		}
		if (this->normalTexture)
		{
			glActiveTexture(GL_TEXTURE5);
			glBindTexture(GL_TEXTURE_2D, this->normalTexture);
			shader.setInt(5, "material.normal");
		}

		if (this->maxFrames) {
			shader.setFloat(float(this->actualFrame) / this->maxFrames, "texMov");
			this->actualFrame = (this->actualFrame + 1) % this->maxFrames;
		}
		else {
			shader.setFloat(0.f, "texMov");
		}

	}

	void openStream()
	{
		glCreateVertexArrays(1, &this->idVAO);
		glBindVertexArray(this->idVAO);

		glGenBuffers(1, &this->idVBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->idVBO);
		glBufferData(GL_ARRAY_BUFFER, this->vertsCount * sizeof(Vertex), &this->verts[0], GL_STATIC_DRAW);
		if (this->indicesCount) {
			glGenBuffers(1, &this->idVBO_idx);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->idVBO_idx);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indicesCount * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);
		}

		//pos
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, v_position)));

		//normal
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, v_normal)));

		//uv
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, v_tex)));

		glBindVertexArray(0);
	}

public:

	BaseMesh() {}

	BaseMesh(
		std::vector<Vertex> vertices,
		std::vector<GLuint> indices,
		GLuint defaultTex = 0,
		int frames = 0,
		GLuint albedoTex = 0,
		GLuint roughnessTex = 0,
		GLuint aoTex = 0,
		GLuint metallicTex = 0,
		GLuint normalTex = 0
	)
	{
		this->verts = vertices;
		this->indices = indices;
		this->vertsCount = verts.size();
		this->indicesCount = indices.size();
		this->maxFrames = frames;

		if (defaultTex)
			this->defaultTexture = defaultTex;
		if (albedoTex)
			this->albedoTexture = albedoTex;
		if (roughnessTex)
			this->roughnessTexture = roughnessTex;
		if (aoTex)
			this->aoTexture = aoTex;
		if (normalTex)
			this->normalTexture = normalTex;
		if (metallicTex)
			this->metallicTexture = metallicTex;

		this->openStream();
	}

	~BaseMesh()
	{

	}
	void render(Shader &shader)
	{
		if (!IsActualProgram(shader.getId()))
			shader.use();
		
		this->updateTextureUniforms(shader);
	}

	void deallocateMemory()
	{
		if (idVAO)
			glDeleteVertexArrays(1, &this->idVAO);
		if (idVBO)
			glDeleteBuffers(1, &this->idVBO);
		if (idVBO_idx)
			glDeleteBuffers(1, &this->idVBO_idx);
		if (this->defaultTexture)
			delete& this->defaultTexture;
		if (this->albedoTexture)
			delete& this->albedoTexture;
		if (this->roughnessTexture)
			delete& this->roughnessTexture;
		if (this->roughnessTexture)
			delete& this->roughnessTexture;
		if (this->aoTexture)
			delete& this->aoTexture;
		if (this->normalTexture)
			delete& this->normalTexture;
	}


};