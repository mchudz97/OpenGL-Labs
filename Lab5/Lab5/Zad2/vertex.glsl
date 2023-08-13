#version 330

layout( location = 0 ) in vec4 inPosition;
layout( location = 1 ) in vec2 inUV;

uniform mat4 matProjView;
uniform float uvPos;
out vec2 UV;
void main()
{
	UV = vec2(inUV[0] + uvPos, inUV[1]);
	gl_Position =  inPosition;

}