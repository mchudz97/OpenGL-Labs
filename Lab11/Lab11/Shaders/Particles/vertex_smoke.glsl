#version 330

layout( location = 0 ) in vec3 inPosition;
layout( location = 1 ) in vec3 inNorm;
layout( location = 2 ) in vec2 inUV;
layout( location = 3 ) in mat4 modelTransformInst;
uniform mat4 matProj;
uniform mat4 matView;
uniform vec3 origin;
uniform float maxH;
uniform float actualStep;
out vec2 UV;
out float ratio;
void main()
{
	float newY = modelTransformInst[3][1] + actualStep;
	if(newY > origin.y + maxH)
		newY -= maxH;
	mat4 newTransform = modelTransformInst;
	newTransform[3][1] =  newY;
	vec3 fragPos = vec3(newTransform * vec4(inPosition, 1.0));

	ratio = (fragPos.y - origin.y) / maxH;
	float ratioSize = .5 + pow(ratio * 3, 2);
	mat4 MV = matView * newTransform;
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			if(i == j)
				MV[i][j] = ratioSize;
			else
				MV[i][j] = 0.f;
		}
	}
	UV = inUV;
	gl_Position = matProj * MV * vec4(inPosition, 1.0);
}