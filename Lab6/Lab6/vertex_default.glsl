#version 330

layout( location = 0 ) in vec3 inPosition;
layout( location = 1 ) in vec3 inNormal;
layout( location = 2 ) in vec2 inUV;
layout( location = 3 ) in vec3 inTangent;
layout( location = 4 ) in vec3 inBitangent;


uniform mat4 modelTransform;
uniform mat4 matProj;
uniform mat4 matView;
uniform float texMov;

out vec3 fragPos;
smooth out vec2 UV;
out mat3 vTBN;
void main()
{
	mat3 MV = mat3(matView * modelTransform); 
	fragPos = vec3(modelTransform * vec4(inPosition, 1.0));
	vec3 T = normalize(vec3(modelTransform * vec4(inTangent, 0.0)));
	vec3 N = normalize(vec3(modelTransform * vec4(inNormal, 0.0)));
	vec3 B = normalize(vec3(modelTransform * vec4(inBitangent, 0.0)));
	vTBN = mat3(T, B, N);
	UV = vec2(inUV[0] + texMov, inUV[1]);
	gl_Position = matProj * matView * modelTransform * vec4(inPosition, 1.0);
	
}