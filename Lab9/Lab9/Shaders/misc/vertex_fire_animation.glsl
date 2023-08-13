#version 330

layout( location = 0 ) in vec3 inPosition;
layout( location = 2 ) in vec2 inUV;
layout( location = 3 ) in mat4 modelTransformInst;
uniform mat4 matProj;
uniform mat4 matView;
uniform float texMov;
out vec2 UV;

void main()
{
	mat4 MV = matView * modelTransformInst;
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			if(i == j)
				MV[i][j] = 1.f;
			else
				MV[i][j] = 0.f;
		}
	}
	UV = vec2(inUV[0] + texMov, inUV[1]);
	gl_Position = matProj * MV * vec4(inPosition, 1.0);
}