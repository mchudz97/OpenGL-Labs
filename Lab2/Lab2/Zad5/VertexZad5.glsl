#version 330 core

// Dane pobrane z VAO
in vec4 inPosition;

// Predefiniowane dane wejsciowe
//in int gl_VertexID;
//in int gl_InstanceID;

// Dane wyjsciowe (kolejny etap potoku)
//out vec4 gl_Position;


void main()
{
	vec4 rotatedVec = vec4(-inPosition[1], -inPosition[0], inPosition[2], inPosition[3]);

	gl_Position = rotatedVec;
}