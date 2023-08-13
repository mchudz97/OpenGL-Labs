#include <stdio.h>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "shader_stuff.h"
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include "Mesh.h"
#include "teapot.h"



	
int WIDTH = 600;
int HEIGHT = 400;
int RESOLUTION[] = { WIDTH, HEIGHT };

vector<Mesh> meshes;
GLuint idProgram;
glm::mat4 matView;
glm::mat4 matProj;

bool polygonLined = false;
float rot = .1f;
float camRot = .0f;
void DisplayScene()
{
	matView = glm::mat4(1.0);
	matView = glm::lookAt(glm::vec3(sin(camRot), 0.0, cos(camRot)), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(idProgram);
	glUniform1iv(glGetUniformLocation(idProgram, "uRESOLUTION"), 2, RESOLUTION);
	glUniformMatrix4fv(glGetUniformLocation(idProgram, "matProj"), 1, GL_FALSE, glm::value_ptr(matProj));
	glUniformMatrix4fv(glGetUniformLocation(idProgram, "matView"), 1, GL_FALSE, glm::value_ptr(matView));
	

	for (Mesh x : meshes) {
		x.render(idProgram);
	}
	

	glUseProgram(0);
	glBindVertexArray(0);
	glutSwapBuffers();

}

void Initialize()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(.7f, .7f, .7f, 1.f);

	idProgram = glCreateProgram();

	glAttachShader(idProgram, LoadShader(GL_VERTEX_SHADER, "Vertex.glsl"));
	glAttachShader(idProgram, LoadShader(GL_FRAGMENT_SHADER, "Fragment.glsl"));

	LinkAndValidateProgram(idProgram);

	meshes.push_back(Mesh(teapotPosition, teapotNormal, teapotIndices, TEAPOT_VERTEX_COUNT, TEAPOT_INDICES_COUNT));
	
}
void Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	matProj = glm::perspective(glm::radians(90.0f), width / (float)height, 0.1f, 5.0f);
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
		if (!polygonLined) 
		{
			polygonLined = true;
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			polygonLined = false;
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		// odswiezenie ekranu
		glutPostRedisplay();
		break;

	case 's':
		meshes[0].rotate(glm::vec3(-1.f, 0.f, 0.f), rot);
		glutPostRedisplay();
		break;

	case 'w':
		meshes[0].rotate(glm::vec3(1.f, 0.f, 0.f), rot);
		glutPostRedisplay();
		break;
	case 'a':
		meshes[0].rotate(glm::vec3(0.f, -1.f, 0.f), rot);
		glutPostRedisplay();
		break;
	case 'd':
		meshes[0].rotate(glm::vec3(0.f, 1.f, 0.f), rot);
		glutPostRedisplay();
		break;
	case '=':
		meshes[0].translate(glm::vec3(0.f, 0.f, .1f));
		glutPostRedisplay();
		break;
	case '-':
		meshes[0].translate(glm::vec3(0.f, 0.f, -.1f));
		glutPostRedisplay();
		break;
	}
	
}
void KeyboardSpecial(int key, int x, int y) {
	switch (key) 
	{
	case GLUT_KEY_UP:

		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:
		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT:
		camRot -= .1f;
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:
		camRot += .1f;
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
	glutSpecialFunc(KeyboardSpecial);


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
	for (Mesh m : meshes) 
	{
		m.deallocateMemory();
	}


	return 0;
}

