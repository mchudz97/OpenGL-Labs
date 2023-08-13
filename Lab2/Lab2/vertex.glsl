// Vertex shader
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
	gl_Position = inPosition;
}