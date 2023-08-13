#version 330 core
layout (location = 0) in vec4 inPos;
uniform mat4 matView;
uniform mat4 matProj;
uniform mat4 matModel;
out vec2 TexCoords;
void main()
{	
	mat4 MV = matView * matModel;
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			if(i == j)
				MV[i][j] = 1.f;
			else
				MV[i][j] = 0.f;
		}
	}
	TexCoords = inPos.zw;
	gl_Position = matProj * MV * vec4(inPos.xy, 0.0, 1.0);
}