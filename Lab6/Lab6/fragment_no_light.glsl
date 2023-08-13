#version 150 core

in vec2 UV;
uniform int alphaTex;
out vec4 outColor;
uniform sampler2D tex;
void main()
{
	outColor = texture(tex, UV);
}