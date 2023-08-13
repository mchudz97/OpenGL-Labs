#version 330 core
layout (location = 0) in vec3 inPos;

uniform mat4 modelTransform;

void main()
{
    gl_Position = modelTransform * vec4(inPos, 1.0);
}