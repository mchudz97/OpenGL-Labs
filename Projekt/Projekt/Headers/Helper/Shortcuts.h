#pragma once
void setVP(Shader shader, glm::mat4 matView, glm::mat4 matProj, glm::vec3 camPos) 
{
	shader.setVec3(camPos, "camPos");
	shader.setMat4(matProj, "matProj");
	shader.setMat4(matView, "matView");
}
void setVP(Shader shader, glm::mat4 matView, glm::mat4 matProj)
{
	shader.setMat4(matProj, "matProj");
	shader.setMat4(matView, "matView");
}