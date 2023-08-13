#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "house.h"
#include "shader_stuff.h"
#include <iostream>

// ---------------------------------------
// Identyfikatory obiektow

GLuint idProgram;	// programu
GLuint idVAO;		// tablic wierzcholkow
GLuint idVBO_coord;	// bufora wierzcholkow
GLuint idVBO_res;	// bufora koloru
int WIDTH = 640;
int HEIGHT = 480;
int RESOLUTION[] = { WIDTH, HEIGHT };
//GLfloat triangles[AMOUNT*VERTS*DIMS] = {};
// ---------------------------------------



// ---------------------------------------
void DisplayScene()
{

	glClear(GL_COLOR_BUFFER_BIT);

	// Wlaczenie VAO i programu
	glBindVertexArray(idVAO);
	glUseProgram(idProgram);
	
	// Generowanie obiektow na ekranie
	glDrawArrays(GL_TRIANGLES, 0, sizeof(Mesh_Vertices)/sizeof(float));


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
	

	glAttachShader(idProgram, LoadShader(GL_VERTEX_SHADER, "VertexZad5.glsl"));
	glAttachShader(idProgram, LoadShader(GL_FRAGMENT_SHADER, "FragmentZad5.glsl"));

	LinkAndValidateProgram(idProgram);


	// 2. Vertex arrays
	glGenVertexArrays(1, &idVAO);
	glBindVertexArray(idVAO);

	// Bufor na wspolrzedne wierzcholkow
	glGenBuffers(1, &idVBO_coord);
	glBindBuffer(GL_ARRAY_BUFFER, idVBO_coord);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Mesh_Vertices), Mesh_Vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	glUseProgram(idProgram);
	GLint location = glGetUniformLocation(idProgram, "uRESOLUTION");
	if (location >= 0) {
		glUniform1iv(location, 2, RESOLUTION);
	}

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
	

	srand(time(NULL));
	/*for (int i = 0; i < AMOUNT * VERTS * DIMS; i++) {		
		if(i % 2 == 0)
			triangles[i] = (float(rand() % WIDTH - WIDTH / 2) * 2 / WIDTH);
		else
			triangles[i] = (float(rand() % HEIGHT - HEIGHT / 2) * 2 / HEIGHT);	
	}*/
	
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