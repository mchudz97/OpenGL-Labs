#version 330
layout( location = 0 ) in vec3 inPosition;
layout( location = 1 ) in vec3 inNormal;
layout( location = 2 ) in vec2 inUV;

uniform mat4 modelTransform;
uniform mat4 matProj;
uniform mat4 matView;
out vec2 UV;

void main()
{

	UV = inUV;
	gl_Position = matProj * matView * modelTransform * vec4(inPosition, 1.0);
	
}