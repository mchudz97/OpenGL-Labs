#version 150 core

out vec4 outColor;
in vec2 UV;
uniform sampler2D tex1;
uniform sampler2D tex2;

void main()
{

	float pos = abs(sin(gl_FragCoord[0]) * cos(gl_FragCoord[1]));
	outColor = mix(texture(tex1, UV), texture(tex2, UV), pos);
	
}