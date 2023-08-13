// Fragment shader
#version 330 core

// Predefiniowane dane wejsciowe
//in  vec4 gl_FragCoord;
//in  int  gl_PrimitiveID;

// Dane wyjsciowe
out vec4 outColor;


void main()
{
	
	float selected;
	if(gl_PrimitiveID%2 == 0){
		outColor = vec4(.9, .8, .7, 1.0);
		selected = gl_FragCoord.x / 300;
	}

	else{
		outColor = vec4(.7, .8, .9, 1.0);
		selected = gl_FragCoord.y / 200;
	}

	outColor[0] *= selected; 
	outColor[1] *= selected; 
	outColor[2] *= selected; 
}