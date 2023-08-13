#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "shader_stuff.h"
#include "Co-to-jest.h"
#include <iostream>

// ---------------------------------------
// Identyfikatory obiektow

GLuint idProgram;	// programu
GLuint idVAO;		// tablic wierzcholkow
GLuint idVBO_coord;	// bufora wierzcholkow
GLuint idVBO_res;	// bufora koloru
GLuint idVBO_idx;
int WIDTH = 600;
int HEIGHT = 400;
int RESOLUTION[] = { WIDTH, HEIGHT };
// ---------------------------------------


// ---------------------------------------
void DisplayScene()
{

	glClear(GL_COLOR_BUFFER_BIT);

	
		// Wlaczenie VAO i programu
	glBindVertexArray(idVAO);
	glUseProgram(idProgram);
	glUniform1iv(glGetUniformLocation(idProgram, "uRESOLUTION"), 2, RESOLUTION);
	// Generowanie obiektow na ekranie
	
	glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, NULL);


	// Wylaczanie
	glUseProgram(0);
	glBindVertexArray(0);
	glutSwapBuffers();
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
	glGenVertexArrays(1, &idVAO);
	glBindVertexArray(idVAO);

	// Bufor na wspolrzedne wierzcholkow
	glGenBuffers(1, &idVBO_coord);
	glBindBuffer(GL_ARRAY_BUFFER, idVBO_coord);
	glBufferData(GL_ARRAY_BUFFER, sizeof(coord), coord, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	

	glGenBuffers(1, &idVBO_idx);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idVBO_idx);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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
	glDeleteVertexArrays(1, &idVBO_coord);
	glDeleteVertexArrays(1, &idVAO);

	return 0;
}