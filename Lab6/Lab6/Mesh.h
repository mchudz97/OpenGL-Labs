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
	Texture* diffuseTexture;
	Texture* specularTexture;
	Texture* normalTexture;
	

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

		//tangents
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, v_tangent)));
		//tangents
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, v_bitangent)));

		glBindVertexArray(0);
	}

public:

	Mesh(
		vector<Vertex>* vertices,
		vector<GLuint>* indices = NULL,
		const char* defaultPath = NULL,
		int frames = 0,
		const char* diffusePath = NULL,
		const char* specularPath = NULL,
		const char* normalPath = NULL,
		const bool removeBlack = false
	)
	{
		this->verts = *vertices;
		this->indices = *indices;
		this->transform = glm::mat4(1.0f);
		this->vertsCount = verts.size();
		if (indices)
			this->indicesCount = (*indices).size();
		
		if (defaultPath) 
		{
			if (removeBlack)
				this->defaultTexture = new Texture(defaultPath, frames, textureType::REMOVE_BLACK);

			else
				this->defaultTexture = new Texture(defaultPath, frames);
		}
		if (diffusePath)
			this->diffuseTexture = new Texture(diffusePath, frames, textureType::DIFFUSE);
		if (specularPath)
			this->specularTexture = new Texture(specularPath, frames, textureType::SPECULAR);
		if (normalPath)
			this->specularTexture = new Texture(normalPath, frames, textureType::BUMP);
		this->openStream();
	}
	~Mesh()
	{

	}

	void render(GLuint idProgram)
	{
		updateUniforms(idProgram);
		glBindVertexArray(this->idVAO);
		if(this->defaultTexture)
			this->defaultTexture->activate(idProgram);
		if (this->diffuseTexture)
			this->diffuseTexture->activate(idProgram);
		if (this->specularTexture)
			this->specularTexture->activate(idProgram);
		if (this->normalTexture)
			this->normalTexture->activate(idProgram);

		if (indicesCount)
			glDrawElements(GL_TRIANGLES, this->indicesCount, GL_UNSIGNED_INT, 0);
		else
			glDrawArrays(GL_TRIANGLES, 0, this->vertsCount * sizeof(Vertex));

		glBindVertexArray(0);
		if (this->defaultTexture)
			this->defaultTexture->deactivate();
		if (this->diffuseTexture)
			this->diffuseTexture->deactivate();
		if (this->specularTexture)
			this->specularTexture->deactivate();
		if (this->normalTexture)
			this->normalTexture->deactivate();
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
		if (this->diffuseTexture)
			delete& this->diffuseTexture;
		if (this->specularTexture)
			delete& this->specularTexture;
		if (this->normalTexture)
			delete& this->normalTexture;
	}

	glm::mat4 getTransform()
	{
		return this->transform;
	}
};
