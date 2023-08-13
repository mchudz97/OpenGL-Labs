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
	Texture *texture;

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

		//color
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, v_color)));

		//normal
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, v_normal)));

		//uv
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, v_tex)));

		glBindVertexArray(0);
	}
	void loadTexture(const char* path) {
		int tex_width, tex_height, n;
		unsigned char* tex_data;
		stbi_set_flip_vertically_on_load(true); // tylko raz aby ...

		tex_data = stbi_load(path, &tex_width, &tex_height, &n, 0);
		if (tex_data == NULL)
		{
			printf("Image can't be loaded!\n");
			exit(1);
		}
		glGenTextures(1, &this->idTex);
		glBindTexture(GL_TEXTURE_2D, this->idTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data);
		stbi_image_free(tex_data);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

public:
	/*Mesh(GLfloat* vertices,
		GLuint vertAmount,
		GLfloat* colors = NULL,
		GLfloat* normals = NULL,
		GLuint* indices = NULL,
		GLuint indicesAmount = 0)
	{
		this->vertsCount = vertAmount;
		this->indicesCount = indicesAmount;
		this->transform = glm::mat4(1.0f);
		if (colors == NULL || normals == NULL)
		{
			if (colors != NULL)
			{
				for (size_t i = 0; i < vertAmount * 3; i += 3)
				{
					Vertex v;
					v.v_position = glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]);
					v.v_color = glm::vec3(colors[i], colors[i + 1], colors[i + 2]);
					this->verts.push_back(v);
				}
			}

			else
			{
				for (size_t i = 0; i < vertAmount * 3; i += 3)
				{
					Vertex v;
					v.v_position = glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]);
					v.v_normal = glm::vec3(normals[i], normals[i + 1], normals[i + 2]);
					this->verts.push_back(v);
				}
			}
		}
		else
		{
			for (size_t i = 0; i < vertAmount * 3; i += 3) {
				Vertex v;
				v.v_position = glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]);
				v.v_color = glm::vec3(normals[i], normals[i + 1], normals[i + 2]);
				v.v_normal = glm::vec3(normals[i], normals[i + 1], normals[i + 2]);
				this->verts.push_back(v);
			}
		}

		for (size_t i = 0; i < indicesAmount; i++) {
			this->indices.push_back(indices[i]);
		}

		openStream();

	}*/
	Mesh(vector<Vertex> *vertices, vector<GLuint> *indices = NULL, const char* path = NULL, int texFrames = 0, bool removeBlack = false) 
	{
		this->verts = *vertices;
		this->indices = *indices;
		this->transform = glm::mat4(1.0f);
		this->vertsCount = verts.size();
		if (indices)
			this->indicesCount = (*indices).size();
		this->openStream();
		this->texture = new Texture(path, texFrames, removeBlack);
	}
	~Mesh()
	{

	}

	void render(GLuint idProgram)
	{
		updateUniforms(idProgram);
		glBindVertexArray(this->idVAO);
		this->texture->display(idProgram);
		if (this->texture->RemovesBlack()) {
			glEnable(GL_BLEND);
		}
		else {
			glDisable(GL_BLEND);
		}
		if (indicesCount)
			glDrawElements(GL_TRIANGLES, this->indicesCount, GL_UNSIGNED_INT, 0);
		else
			glDrawArrays(GL_TRIANGLES, 0, this->vertsCount * sizeof(Vertex));

		glBindVertexArray(0);

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
		if (texture) {
			delete& texture;
		}
	}

	glm::mat4 getTransform()
	{
		return this->transform;
	}
};
