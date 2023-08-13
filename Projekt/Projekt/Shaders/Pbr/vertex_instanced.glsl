#version 330

layout( location = 0 ) in vec3 inPosition;
layout( location = 1 ) in vec3 inNormal;
layout( location = 2 ) in vec2 inUV;
layout( location = 3 ) in mat4 modelTransformInst;

uniform mat4 lightSpaceMatrix;
uniform mat4 matProj;
uniform mat4 matView;
uniform float texMov;

out vec3 fragPos;
out vec2 UV;
out vec3 Normal;
out vec4 fragPosLightSpace;

void main()
{
	Normal = inNormal;
	fragPos = vec3(modelTransformInst * vec4(inPosition, 1.0));
	UV = vec2(inUV[0] + texMov, inUV[1]);
	gl_Position = matProj * matView * modelTransformInst * vec4(inPosition, 1.0);
	
}