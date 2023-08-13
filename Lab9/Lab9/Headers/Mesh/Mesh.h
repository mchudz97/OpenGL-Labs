#pragma once
using namespace std;

class Mesh : public BaseMesh
{
private:

	glm::mat4 transform;

	void updateUniforms(Shader shader)
	{
		shader.setMat4(this->transform, "modelTransform");
	}

public:
	
	Mesh(){}

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
		) : BaseMesh(vertices, indices, defaultTex, frames, albedoTex, roughnessTex, aoTex, metallicTex, normalTex)
	{
		this->transform = glm::mat4(1.0f);		
	}

	~Mesh()
	{

	}

	void render(Shader shader)
	{
		BaseMesh::render(shader);
		this->updateUniforms(shader);
		glBindVertexArray(this->idVAO);

		if (indicesCount)
			glDrawElements(GL_TRIANGLES, this->indicesCount, GL_UNSIGNED_INT, 0);
		else
			glDrawArrays(GL_TRIANGLES, 0, this->vertsCount * sizeof(Vertex));

		glBindVertexArray(0);
		glActiveTexture(0);
	}

	//void staticRender(GLuint idProgram, vector<glm::vec3> positions) {
	//	for (int i = 0; i < positions.size(); i++)
	//	{
	//		this->transform = glm::mat4(1.f);
	//		this->translate(positions[i]);
	//		this->updateUniforms(idProgram);
	//		this->render(idProgram);
	//	}
	//}

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

	glm::mat4 getTransform()
	{
		return this->transform;
	}
};
void exampleTranslationFunc(glm::mat4 &t, float step) {
	
	float range = 10;
	
	t = glm::translate(glm::mat4(1.f), glm::vec3(sin(step) * range, 7, cos(step) * range));
	t = glm::rotate(t, step * 2, glm::vec3(0, 1, 0));

}