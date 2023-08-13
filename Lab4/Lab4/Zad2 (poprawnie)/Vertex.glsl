#version 330 core

layout (location = 0) in vec4 pos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;

uniform mat4 modelTransform;
uniform mat4 matProj;
uniform mat4 matView;

out vec3 v_normal;
out vec3 v_color;


void main()
{
	v_color = color;
	v_normal = mat3(modelTransform) * normal;
	vec4 newPos = matProj * matView * modelTransform * pos;
	gl_Position = newPos;
}