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
		outColor = vec4(.7, .7, .7, 1.0);
		selected = gl_FragCoord.x / 200;
	}

	else{
		outColor = vec4(.9, .9, .9, 1.0);
		selected = gl_FragCoord.y / 300;
	}

	outColor[0] *= selected; 
	outColor[1] *= selected; 
	outColor[2] *= selected; 
}