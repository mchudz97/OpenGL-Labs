#version 150 core

in vec2 UV;
out vec4 outColor;
uniform sampler2D tex;
void main()
{
	vec4 color = texture(tex, UV);
	if(color.x < .25f && color.y < .25f && color.z < .25f)
	{
		discard;
	}
	outColor = color;
}