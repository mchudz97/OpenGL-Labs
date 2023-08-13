#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 3) in mat4 modelTransformInst;
uniform bool isInstanced;
uniform mat4 modelTransform;
uniform mat4 lightSpaceMatrix;


void main()
{
	mat4 tr;

	if(isInstanced)
		tr = modelTransformInst;
	else
		tr = modelTransform;

    gl_Position = lightSpaceMatrix * tr * vec4(aPos, 1.0);
} 