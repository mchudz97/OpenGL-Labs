#version 150 core

in vec2 UV;
uniform sampler2D tex;
void main()
{

	if(texture(tex, UV).w < .5f)
		discard;

}