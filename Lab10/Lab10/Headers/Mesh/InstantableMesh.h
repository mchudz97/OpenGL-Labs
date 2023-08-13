#pragma once
#include <algorithm>
class InstantableMesh : public BaseMesh
{
private:
	GLuint matVBO = 0;
	std::vector<glm::mat4> matrices;
	bool refreshBuffer = false;
	void setupMatrices() 
	{
		if (this->matrices.size() == 0)
			return;
		glBindVertexArray(this->idVAO);
		if (!this->matVBO) {
			glGenBuffers(1, &this->matVBO);
			glBindBuffer(GL_ARRAY_BUFFER, this->matVBO);
			glBufferData(GL_ARRAY_BUFFER, this->matrices.size() * sizeof(glm::mat4), &this->matrices[0], GL_STATIC_DRAW);
		}
		else 
		{
			//case when we update matrices, we resize buffer and change drawing type
			glBindBuffer(GL_ARRAY_BUFFER, this->matVBO);
			glBufferData(GL_ARRAY_BUFFER, this->matrices.size() * sizeof(glm::mat4), &this->matrices[0], GL_DYNAMIC_DRAW);
		}
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);

	}

public:
	InstantableMesh(){}
	InstantableMesh(
		std::vector<Vertex> vertices,
		std::vector<GLuint> indices,
		std::vector<glm::mat4> mats,
		GLuint defaultTex = 0,
		int frames = 0,
		GLuint albedoTex = 0,
		GLuint roughnessTex = 0,
		GLuint aoTex = 0,
		GLuint metallicTex = 0,
		GLuint normalTex = 0) 
		: BaseMesh(vertices, indices, defaultTex, frames, albedoTex, roughnessTex, aoTex, metallicTex, normalTex)
	{
		this->matrices = mats;
		this->setupMatrices();
	}

	void render(Shader &shader) 
	{
		if (refreshBuffer) 
		{
			this->setupMatrices();
			this->refreshBuffer = false;
		}
		BaseMesh::render(shader);
		glBindVertexArray(this->idVAO);
		glDrawElementsInstanced(GL_TRIANGLES, this->indicesCount, GL_UNSIGNED_INT, 0, this->matrices.size());
		glBindVertexArray(0);

	}

	void shadowRender(Shader& shader) 
	{
		if (refreshBuffer)
		{
			this->setupMatrices();
			this->refreshBuffer = false;
		}

		shader.setBool(true, "isInstanced");
		
		glBindVertexArray(this->idVAO);
		glDrawElementsInstanced(GL_TRIANGLES, this->indicesCount, GL_UNSIGNED_INT, 0, this->matrices.size());
		glBindVertexArray(0);
	}

	void renderToMap(Shader& shader)
	{
		if (refreshBuffer)
		{
			this->setupMatrices();
			this->refreshBuffer = false;
		}
		BaseMesh::renderToMap(shader);
		shader.setBool(true, "isInstanced");

		glBindVertexArray(this->idVAO);
		glDrawElementsInstanced(GL_TRIANGLES, this->indicesCount, GL_UNSIGNED_INT, 0, this->matrices.size());
		glBindVertexArray(0);
	}
	
	void attachMatrices(std::vector<glm::mat4> mats) 
	{
		this->matrices.insert(this->matrices.end(), mats.begin(), mats.end());
		this->refreshBuffer = true;
	}
	
	void translate(glm::vec3 vec) 
	{
		for (glm::mat4& mat : this->matrices)
			mat = glm::translate(mat, vec);
		
		this->refreshBuffer = true;
	}
	
	std::vector<glm::mat4> getMatrices() 
	{
		return this->matrices;
	}
	
	void setMatrices(std::vector<glm::mat4> mats) 
	{
		this->matrices = mats;
		this->refreshBuffer = true;
	}

	void sortMatrices(glm::vec3 cameraPos) {

		std::sort(this->matrices.begin(), this->matrices.end(), [&cameraPos](const glm::mat4 f, const glm::mat4 s) 
			{
				float dist1 = glm::distance(glm::vec3(f[3][0], f[3][1], f[3][2]), cameraPos);
				float dist2 = glm::distance(glm::vec3(s[3][0], s[3][1], s[3][2]), cameraPos);
				return dist1 > dist2;
			});
		

		this->refreshBuffer = true;
	}
};