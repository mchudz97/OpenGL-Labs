#version 150 core

out vec4 outColor;
in vec2 UV;
uniform sampler2D minimap;
void main()
{
	vec4 tex = texture(minimap, UV);
	if(dot(tex.rgb, tex.rgb) <= .0f)
		discard;
	outColor = tex;
}