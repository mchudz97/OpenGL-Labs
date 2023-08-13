#pragma once
bool IsActualProgram(GLuint shaderId) 
{
	GLint currProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currProgram);
	if (currProgram != shaderId) {
		return false;
	}
	return true;
}