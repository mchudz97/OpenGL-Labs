#pragma once
using namespace std;

class Mesh
{
private:

	GLuint idVAO;
	GLuint idVBO;
	GLuint idVBO_idx;
	
	vector<Vertex> verts;
	vector<GLuint> indices;

	GLuint vertsCount;
	GLuint indicesCount;

	glm::mat4 transform;

	GLuint defaultTexture;

	GLuint albedoTexture;
	GLuint roughnessTexture;
	GLuint aoTexture;
	GLuint normalTexture;
	GLuint metallicTexture;

	GLuint maxFrames;
	GLuint actualFrame;
	float a;
	void updateUniforms(GLuint idProgram)
	{
		glUniformMatrix4fv(glGetUniformLocation(idProgram, "modelTransform"), 1, GL_FALSE, glm::value_ptr(this->transform));
	}

	void updateTextureUniforms(GLuint idProgram) 
	{
		if (this->defaultTexture) 
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, this->defaultTexture);
			glUniform1i(glGetUniformLocation(idProgram, "tex"), 0);
		}
		if (this->albedoTexture) 
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, this->albedoTexture);
			glUniform1i(glGetUniformLocation(idProgram, "material.albedo"), 1);
		}
		if (this->roughnessTexture) 
		{
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, this->roughnessTexture);
			glUniform1i(glGetUniformLocation(idProgram, "material.roughness"), 2);
		}
		if (this->aoTexture) 
		{
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, this->aoTexture);
			glUniform1i(glGetUniformLocation(idProgram, "material.ao"), 3);
		}
		if (this->metallicTexture) 
		{
			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D, this->metallicTexture);
			glUniform1i(glGetUniformLocation(idProgram, "material.metallic"), 4);
		}
		if (this->normalTexture) 
		{
			glActiveTexture(GL_TEXTURE5);
			glBindTexture(GL_TEXTURE_2D, this->normalTexture);
			glUniform1i(glGetUniformLocation(idProgram, "material.normal"), 5);
		}

		if (this->maxFrames) {
			glUniform1f(glGetUniformLocation(idProgram, "texMov"), float(this->actualFrame) / this->maxFrames);
			this->actualFrame = (this->actualFrame + 1) % this->maxFrames;
		}
		else {
			glUniform1f(glGetUniformLocation(idProgram, "texMov"), 0.0f);
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

	Mesh(
		vector<Vertex> vertices,
		vector<GLuint> indices,
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
		this->transform = glm::mat4(1.0f);
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
	~Mesh()
	{

	}

	void render(GLuint idProgram)
	{
		updateUniforms(idProgram);
		glBindVertexArray(this->idVAO);
		this->updateTextureUniforms(idProgram);

		if (indicesCount)
			glDrawElements(GL_TRIANGLES, this->indicesCount, GL_UNSIGNED_INT, 0);
		else
			glDrawArrays(GL_TRIANGLES, 0, this->vertsCount * sizeof(Vertex));

		glBindVertexArray(0);
		glActiveTexture(0);
	}

	void staticRender(GLuint idProgram, vector<glm::vec3> positions) {
		for (int i = 0; i < positions.size(); i++)
		{
			this->transform = glm::mat4(1.f);
			this->translate(positions[i]);
			this->updateUniforms(idProgram);
			this->render(idProgram);
		}
	}

	void rotate(glm::vec3 direction, GLfloat value)
	{
		this->transform = glm::rotate(this->transform, value, direction);
	}


	void translate(glm::vec3 value)
	{
		this->transform = glm::translate(this->transform, value);
	}
	void setTranslation(float value, void(*f)(glm::mat4&, float)) {
		f(this->transform, value);
	}

	void scale(glm::vec3 value)
	{
		this->transform = glm::scale(this->transform, value);
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

	glm::mat4 getTransform()
	{
		return this->transform;
	}
};
void exampleTranslationFunc(glm::mat4 &t, float step) {
	
	float range = 10;
	
	t = glm::translate(glm::mat4(1.f), glm::vec3(sin(step) * range, 7, cos(step) * range));
	t = glm::rotate(t, step * 2, glm::vec3(1, 1, 1));

}