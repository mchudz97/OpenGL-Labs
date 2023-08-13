#version 330

layout( location = 0 ) in vec3 inPosition;
layout( location = 1 ) in vec2 inUV;
layout( location = 3 ) in mat4 modelTransformInst;
uniform mat4 modelTransform;
uniform mat4 matProj;
uniform mat4 matView;

uniform bool isInstanced;
out vec2 camXZ;
out vec2 UV;


void main()
{
	mat4 tr;

	if(isInstanced)
		tr = modelTransformInst;
	else
		tr = modelTransform;
	camXZ = vec2(matView[3][0], matView[3][2]);
	gl_Position = matProj * matView * modelTransform * vec4(inPosition, 1.0);

	UV = inUV;
}