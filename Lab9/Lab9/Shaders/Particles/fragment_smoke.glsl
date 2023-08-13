#version 150 core

in vec2 UV;
in float ratio;
out vec4 outColor;
uniform sampler2D tex;
void main()
{
	vec4 color = texture(tex, UV);
	if(color.w < .9f)
	{
		discard;
	}
	outColor = vec4(color.rgb * pow(ratio, 2), .2f + pow((1.f - ratio), 2) * 4 / 2 / 5 );
}