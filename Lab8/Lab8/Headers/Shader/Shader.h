#include <map>
#ifndef SHADER_H
#define SHADER_H
class Shader
{
private:
	GLuint id;
	void setup(const char* vertex, const char* fragment, const char* geometry = NULL)
	{
		this->id = glCreateProgram();
		glAttachShader(this->id, LoadShader(GL_VERTEX_SHADER, vertex));
		glAttachShader(this->id, LoadShader(GL_FRAGMENT_SHADER, fragment));
		if (geometry)
			glAttachShader(this->id, LoadShader(GL_GEOMETRY_SHADER, geometry));
		LinkAndValidateProgram(this->id);
	}
public:

	Shader(const char* vertex, const char* fragment, const char* geometry = NULL)
	{
		this->setup(vertex, fragment, geometry);
	}

	Shader() {}
	~Shader() {}

	GLuint getId()
	{
		return this->id;
	}
	void use() {
		glUseProgram(this->id);
	}
	void display(std::vector<Mesh>& meshes)
	{

		for (Mesh &m : meshes)
		{
			m.render(this->id);
		}
	}
	void display(std::vector<Mesh>& meshes, std::vector<glm::vec3>& positions)
	{
		for (Mesh m : meshes)
		{
			m.staticRender(this->id, positions);
		}
	}

	void displaySorted(std::vector<Mesh> transparentObjects, glm::mat4 cameraPos) {
		map<float, int> objectsSorted;
		glm::vec3 camPos = glm::vec3(cameraPos[3][0], cameraPos[3][1], cameraPos[3][2]);
		for (int i = 0; i < transparentObjects.size(); i++) {
			glm::mat4 objTransform = transparentObjects[i].getTransform();
			glm::vec3 objPos = glm::vec3(objTransform[3][0], objTransform[3][1], objTransform[3][2]);
			float distance = glm::length(camPos - objPos);
			objectsSorted[distance] = i;
		}
		map<float, int>::reverse_iterator it;
		for (it = objectsSorted.rbegin(); it != objectsSorted.rend(); ++it) {
			int actual = it->second;
			transparentObjects[actual].render(this->id);
		}
	}

	void remove() {
		glDeleteProgram(this->id);
	}
	void setInt(GLuint value, const char* name)
	{
		glUniform1i(glGetUniformLocation(this->id, name), value);
	}
	void setFloat(GLfloat value, const char* name) {
		glUniform1f(glGetUniformLocation(this->id, name), value);
	}
	void setMat4(glm::mat4 mat, const char* name)
	{
		glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, GL_FALSE, glm::value_ptr(mat));
	}
	void setVec3(glm::vec3 vec, const char* name)
	{
		glUniform3fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(vec));
	}
};

#endif