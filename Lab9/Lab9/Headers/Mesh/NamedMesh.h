#pragma once
class NamedMesh : public Mesh
{
private:
	CFont nameplateFont;
	float textHeight;
	string name;
public:

	NamedMesh(
		CFont &c,
		float textHeight,
		string name,
		vector<Vertex> vertices,
		vector<GLuint> indices,
		GLuint defaultTex = 0,
		int frames = 0,
		GLuint albedoTex = 0,
		GLuint roughnessTex = 0,
		GLuint aoTex = 0,
		GLuint metallicTex = 0,
		GLuint normalTex = 0
	) : Mesh(vertices, indices, defaultTex, frames, albedoTex, roughnessTex, aoTex, metallicTex, normalTex)
	{
		this->nameplateFont = c;
		this->textHeight = textHeight;
		this->name = name;
	}
	
	void render(Shader sh) 
	{
		Mesh::render(sh);
	}

	void renderText(glm::mat4 matView, glm::mat4 matProj) {
		this->nameplateFont.setTransform(glm::mat4(1.f));
		glm::vec3 tr = glm::vec3(this->getTransform()[3][0], this->getTransform()[3][1] + this->textHeight, this->getTransform()[3][2]);
		this->nameplateFont.translate(tr);
		this->nameplateFont.renderText3D(name, matView, matProj, .02, glm::vec3(.0f, .0f, 1.f));
	}
};