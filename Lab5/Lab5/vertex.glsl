#version 330

layout( location = 0 ) in vec4 inPosition;
layout( location = 1 ) in vec3 inColor;
layout( location = 2 ) in vec3 inNormal;
layout( location = 3 ) in vec2 inUV;

uniform mat4 modelTransform;
uniform mat4 matProj;
uniform mat4 matView;
uniform float texMov;
out vec3 fragPos;
out vec2 UV;
out vec3 normal;
void main()
{
	fragPos = vec3(mat3(modelTransform) * vec3(inPosition));
	normal = mat3(transpose(inverse(modelTransform))) * inNormal;
	UV = vec2(inUV[0] + texMov, inUV[1]);
	gl_Position = matProj * matView * modelTransform * inPosition;
	
}