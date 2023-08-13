#include <map>
//#include "shader_stuff.h"
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
	void setBool(bool state, const char* name) 
	{
		glUniform1i(glGetUniformLocation(this->id, name), state);
	}
};

#endif