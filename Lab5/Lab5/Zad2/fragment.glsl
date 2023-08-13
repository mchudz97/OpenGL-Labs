#version 150 core

out vec4 outColor;
in vec2 UV;
uniform sampler2D tex1;


void main()
{


	outColor = texture(tex1, UV);
	
}