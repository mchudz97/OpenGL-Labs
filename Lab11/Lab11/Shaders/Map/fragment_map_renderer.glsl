#version 150

in vec2 camXZ;
in vec2 UV;

out vec4 outColor;

uniform sampler2D tex;
uniform int res;
void main()
{
	vec4 color = texture( tex, UV );

	vec2 mid = vec2(res/2, res/2);

	if(distance(gl_FragCoord.xy, mid) > res / 2)
		color = vec4(0.f);

	if(distance(gl_FragCoord.xy, mid) < res / 25)
		color = vec4(1.f, 0.f, 0.f, 1.f);

	outColor = color;

}