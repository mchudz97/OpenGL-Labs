// Fragment shader
#version 330 core

// Predefiniowane dane wejsciowe
//in  vec4 gl_FragCoord;
//in  int  gl_PrimitiveID;

// Dane wyjsciowe
out vec4 outColor;
uniform int uRESOLUTION[2];
float random (vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}
void main()
{
	float r,b,g;
	float xPos = gl_FragCoord[0];
	float yPos = gl_FragCoord[1];
	float rand = random(vec2(xPos/float(uRESOLUTION[0]),yPos)/float(uRESOLUTION[0]));
	float aa = float(int(xPos) % 10) / 10.0;
	float bb = float(int(yPos) % 10) / 10.0;

	r = rand * aa;
	g = rand * bb;
	b = r * g;

	outColor = vec4(r, g, b, 1.0);
	
}