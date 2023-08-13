// Fragment shader
#version 330 core

// Predefiniowane dane wejsciowe
//in  vec4 gl_FragCoord;
//in  int  gl_PrimitiveID;

// Dane wyjsciowe
out vec4 outColor;
uniform int uRESOLUTION[2];
void main()
{
	float x = float(uRESOLUTION[0]);
	float y = float(uRESOLUTION[1]);
	float a = float(int(gl_FragCoord[0]) % 10) / 10.0;
	float b = float(int(gl_FragCoord[1]) % 10) / 10.0;
	x = smoothstep(0, 1, gl_FragCoord[0]/x);
	y = smoothstep(0, 1, gl_FragCoord[1]/y);

	outColor = vec4(a*y, b*x, (a+b+x+y)/4, 1.0);
	
}