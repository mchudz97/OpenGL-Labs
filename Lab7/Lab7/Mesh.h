#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Texture.h"
using namespace std;



class Mesh
{
private:
	GLuint idVAO;
	GLuint idVBO;
	GLuint idVBO_idx;
	GLuint idTex;
	vector<Vertex> verts;
	vector<GLuint> indices;

	GLuint vertsCount;
	GLuint indicesCount;

	glm::mat4 transform;

	Texture* defaultTexture;
	Texture* albedoTexture;
	Texture* roughnessTexture;
	Texture* aoTexture;
	Texture* normalTexture;
	Texture* metallicTexture;


	void updateUniforms(GLuint idProgram)
	{
		glUniformMatrix4fv(glGetUniformLocation(idProgram, "modelTransform"), 1, GL_FALSE, glm::value_ptr(this->transform));
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
		vector<Vertex>* vertices,
		vector<GLuint>* indices = NULL,
		Texture* defaultTex = NULL,
		int frames = 0,
		Texture* albedoTex = NULL,
		Texture* roughnessTex = NULL,
		Texture* aoTex = NULL,
		Texture* metallicTex = NULL,
		Texture* normalTex = NULL
	)
	{
		this->verts = *vertices;
		this->indices = *indices;
		this->transform = glm::mat4(1.0f);
		this->vertsCount = verts.size();
		if (indices)
			this->indicesCount = (*indices).size();

		if (defaultTex)
		{		
			this->defaultTexture = defaultTex;
		}
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
		if (this->defaultTexture)
			this->defaultTexture->activate(idProgram);
		if (this->albedoTexture)
			this->albedoTexture->activate(idProgram, textureType::ALBEDO);
		if (this->roughnessTexture)
			this->roughnessTexture->activate(idProgram, textureType::ROUGHNESS);
		if (this->aoTexture)
			this->aoTexture->activate(idProgram, textureType::AO);
		if (this->metallicTexture)
			this->metallicTexture->activate(idProgram, textureType::METAL);
		if (this->normalTexture)
			this->normalTexture->activate(idProgram, textureType::BUMP);

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
		if (idTex)
			glDeleteBuffers(1, &this->idTex);
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