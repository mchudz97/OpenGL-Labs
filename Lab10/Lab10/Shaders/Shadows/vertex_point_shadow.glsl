#version 330 core
layout( location = 0 ) in vec3 inPosition;
layout( location = 3 ) in mat4 modelTransformInst;
uniform bool isInstanced;
uniform mat4 modelTransform;
void main()
{
	mat4 tr;

	if(isInstanced)
		tr = modelTransformInst;
	else
		tr = modelTransform;

	gl_Position = tr * vec4(inPosition, 1.0);
}