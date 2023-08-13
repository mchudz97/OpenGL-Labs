#pragma once

using namespace std;

struct Vertex
{
	glm::vec3 v_position;
	glm::vec3 v_color;
	glm::vec3 v_normal;

};

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

		glGenBuffers(1, &this->idVBO_idx);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->idVBO_idx);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indicesCount * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

		//pos
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, v_position)));
		
		//color
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, v_color)));
		
		//normal
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, v_normal)));



		glBindVertexArray(0);
	}

public:
	Mesh(GLfloat *vertices, GLfloat *normals, GLuint *indices, GLuint vertAmount, GLuint indicesAmount)
	{
		this->vertsCount = vertAmount;
		this->indicesCount = indicesAmount;
		this->transform = glm::mat4(1.0f);
		for (size_t i = 0; i < vertAmount * 3; i += 3) {
			Vertex v;
			v.v_position = glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]);
			v.v_color = glm::vec3(normals[i], normals[i + 1], normals[i + 2]);
			v.v_normal = glm::vec3(normals[i], normals[i + 1], normals[i + 2]);
			this->verts.push_back(v);
		}
		for (size_t i = 0; i < indicesAmount; i++) {
			this->indices.push_back(indices[i]);
		}

		openStream();

	}

	~Mesh()
	{

	}

	void render(GLuint idProgram) 
	{
		updateUniforms(idProgram);
		glBindVertexArray(this->idVAO);
		glDrawElements(GL_TRIANGLES, this->indicesCount, GL_UNSIGNED_INT, 0);
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
	

	void deallocateMemory() {
		if (idVAO) 
			glDeleteVertexArrays(1, &this->idVAO);
		if (idVBO)
			glDeleteBuffers(1, &this->idVBO);
		if (idVBO_idx)
			glDeleteBuffers(1, &this->idVBO_idx);
	}
};