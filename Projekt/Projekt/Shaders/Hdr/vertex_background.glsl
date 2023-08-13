#version 330 core
layout (location = 0) in vec3 inPos;

uniform mat4 matProj;
uniform mat4 matView;

out vec3 WorldPos;

void main()
{
    WorldPos = inPos;

	mat4 rotView = mat4(mat3(matView));
	vec4 clipPos = matProj * rotView * vec4(WorldPos, 1.0);

	gl_Position = clipPos.xyww;
}