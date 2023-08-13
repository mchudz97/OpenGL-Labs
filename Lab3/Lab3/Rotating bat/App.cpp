#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "shader_stuff.h"
#include "ObiektA.h"
#include "ObiektB.h"
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
// ---------------------------------------
// Identyfikatory obiektow

GLuint idProgram;	// programu
GLuint idVAO1;		// tablic wierzcholkow
GLuint idVAO2;		// tablic wierzcholkow
GLuint idVBO_coord1;	// bufora wierzcholkow
GLuint idVBO_coord2;	// bufora wierzcholkow
GLuint idVBO_res;	// bufora koloru
GLuint idVBO_idx1;
GLuint idVBO_idx2;
int WIDTH = 600;
int HEIGHT = 400;
int RESOLUTION[] = { WIDTH, HEIGHT };
// ---------------------------------------
glm::mat4 transform = glm::mat4(1.0f);
float rot1 = 0.1;
// ---------------------------------------


void DisplayScene()
{

	

	glClear(GL_COLOR_BUFFER_BIT);

	
		// Wlaczenie VAO i programu
	
	glUseProgram(idProgram);
	transform = glm::rotate(transform, rot1, glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(glGetUniformLocation(idProgram, "uTransform"), 1, GL_FALSE, glm::value_ptr(transform));
	glUniform1iv(glGetUniformLocation(idProgram, "uRESOLUTION"), 2, RESOLUTION);
	// Generowanie obiektow na ekranie
	glBindVertexArray(idVAO1);
	glDrawElements(GL_TRIANGLES, sizeof(a::idx) / sizeof(a::idx[0]), GL_UNSIGNED_INT, NULL);

	//obj2 w bezruchu
	glUniformMatrix4fv(glGetUniformLocation(idProgram, "uTransform"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
	glBindVertexArray(idVAO2);
	glDrawElements(GL_TRIANGLES, sizeof(b::idx) / sizeof(b::idx[0]), GL_UNSIGNED_INT, NULL);
	// Wylaczanie
	glUseProgram(0);
	glBindVertexArray(0);
	glutSwapBuffers();


	Sleep(1000/60);
	glutPostRedisplay();

}

// ---------------------------------------
void Initialize()
{


	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// 1. Program i shadery
	idProgram = glCreateProgram();


	glAttachShader(idProgram, LoadShader(GL_VERTEX_SHADER, "VertexZ1.glsl"));
	glAttachShader(idProgram, LoadShader(GL_FRAGMENT_SHADER, "FragmentZ1.glsl"));

	LinkAndValidateProgram(idProgram);


	// 2. Vertex arrays
	glGenVertexArrays(1, &idVAO1);
	glBindVertexArray(idVAO1);

	// Bufor na wspolrzedne wierzcholkow
	glGenBuffers(1, &idVBO_coord1);
	glBindBuffer(GL_ARRAY_BUFFER, idVBO_coord1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(a::coord), a::coord, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glGenBuffers(1, &idVBO_idx1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idVBO_idx1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(a::idx), a::idx, GL_STATIC_DRAW);


	glGenVertexArrays(1, &idVAO2);
	glBindVertexArray(idVAO2);

	glGenBuffers(1, &idVBO_coord2);
	glBindBuffer(GL_ARRAY_BUFFER, idVBO_coord2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(b::coord), b::coord, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glGenBuffers(1, &idVBO_idx2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idVBO_idx2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(b::idx), b::idx, GL_STATIC_DRAW);
	
	glBindVertexArray(0);

	
}

// ---------------------------------------
void Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
}

// ---------------------------------------------------
void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:	// ESC key
		exit(0);
		break;

	case ' ':
		printf("SPACE\n");

		// odswiezenie ekranu
		glutPostRedisplay();
		break;

	}
}


// ---------------------------------------------------
int main(int argc, char* argv[])
{



	// GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("OpenGL!");
	glutDisplayFunc(DisplayScene);
	glutReshapeFunc(Reshape);

	// Keyboard
	glutKeyboardFunc(Keyboard);


	// GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		printf("GLEW Error\n");
		exit(1);
	}

	// OpenGL
	if (!GLEW_VERSION_3_2)
	{
		printf("Brak OpenGL 3.2!\n");
		exit(1);
	}


	Initialize();

	glutMainLoop();


	// Cleaning
	glDeleteProgram(idProgram);
	glDeleteVertexArrays(1, &idVBO_coord1);
	glDeleteVertexArrays(1, &idVBO_coord2);
	glDeleteVertexArrays(1, &idVAO1);
	glDeleteVertexArrays(1, &idVAO2);

	return 0;
}