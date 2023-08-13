#version 330 core
layout (location = 0) in vec3 inPos;

out vec3 WorldPos;

uniform mat4 matProj;
uniform mat4 matView;

void main()
{
    WorldPos = inPos;  
    gl_Position =  matProj * matView * vec4(WorldPos, 1.0);
}