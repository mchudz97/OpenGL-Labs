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
#include "cube.h"


glm::vec3 teapotPositions[4] = {
	glm::vec3(4, 2, 4), glm::vec3(4, 0, 0), glm::vec3(-4, 2, 4), glm::vec3(-4, 0, 0)
};

glm::vec3 teapotRotations[4] = {
	glm::vec3(0, .5, 0), glm::vec3(0, .2, 0), glm::vec3(0, 1.5, 0), glm::vec3(0, 2, 0)
};
glm::vec3 cubePositions[4] = {
	glm::vec3(0, 4, 0), glm::vec3(0, -4, 0), glm::vec3(0, 0, 4), glm::vec3(0, 0, -4)
};

glm::vec3 cubeRotations[4] = {
	glm::vec3(0, .5, 0), glm::vec3(0, .2, 0), glm::vec3(0, 1.5, 0), glm::vec3(0, 2, 0)
};

int numOfObj = 4;

int WIDTH = 600;
int HEIGHT = 400;
int RESOLUTION[] = { WIDTH, HEIGHT };

vector<Mesh> meshes;
GLuint idProgram;
glm::mat4 matView;
glm::mat4 matProj;

bool polygonLined = false;
float rot = .1f;
float radius = 3.f;
float camRot = .0f;
float camY = 0.0f;
unsigned currTarget;
void DisplayScene()
{
	glm::vec3 targetPos = glm::vec3(meshes[currTarget].getTransform()[3]);
	matView = glm::mat4(1.0);
	matView = glm::lookAt(glm::vec3(sin(camRot) * radius, camY * radius, cos(camRot) * radius),
		glm::vec3(0,0,0),
		glm::vec3(0.0, 1.0, 0.0));

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::vec3 camDir = normalize(glm::inverse(matView)[2]);
	glUseProgram(idProgram);
	glUniform3fv(glGetUniformLocation(idProgram, "camDir"), 1, &camDir[0]);
	glUniformMatrix4fv(glGetUniformLocation(idProgram, "matProj"), 1, GL_FALSE, glm::value_ptr(matProj));
	glUniformMatrix4fv(glGetUniformLocation(idProgram, "matView"), 1, GL_FALSE, glm::value_ptr(matView));
	glUniform1iv(glGetUniformLocation(idProgram, "uRESOLUTION"), 2, RESOLUTION);

	for (int i = 0; i < numOfObj; i++) {
		glUniform1i(glGetUniformLocation(idProgram, "mode"), i);
		meshes[0].translate(teapotPositions[i]);
		meshes[0].rotate(teapotRotations[i], 1.f);
		meshes[0].render(idProgram);
		meshes[0].setTransform(glm::mat4(1.f));
	}
	for (int i = 0; i < numOfObj; i++) {
		glUniform1i(glGetUniformLocation(idProgram, "mode"), i);
		meshes[1].translate(cubePositions[i]);
		meshes[1].rotate(cubeRotations[i], .5f);
		meshes[1].render(idProgram);
		meshes[1].setTransform(glm::mat4(.5f));
	}

	glUseProgram(0);
	glBindVertexArray(0);
	glutSwapBuffers();

}

void Initialize()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(.2f, .2f, .2f, 1.f);

	idProgram = glCreateProgram();

	glAttachShader(idProgram, LoadShader(GL_VERTEX_SHADER, "Vertex.glsl"));
	glAttachShader(idProgram, LoadShader(GL_FRAGMENT_SHADER, "Fragment.glsl"));

	LinkAndValidateProgram(idProgram);

	meshes.push_back(Mesh(teapotPosition, TEAPOT_VERTEX_COUNT, teapotNormal, teapotNormal, teapotIndices,  TEAPOT_INDICES_COUNT));
	meshes.push_back(Mesh(coord, 12 * 3, color));
	
	
}
void Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	matProj = glm::perspective(glm::radians(90.0f), width / (float)height, 0.1f, 14.0f);
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
		camY -= 0.1;
		glutPostRedisplay();
		break;

	case 'w':
		camY += 0.1;
		glutPostRedisplay();
		break;
	case 'a':
		camRot -= .1f;
		glutPostRedisplay();
		break;
	case 'd':
		camRot += .1f;
		glutPostRedisplay();
		break;
	case 'q':
		radius += .1f;
		glutPostRedisplay();
		break;
	case 'e':
		radius -= .1f;
		glutPostRedisplay();
		break;
	}
	
}
void KeyboardSpecial(int key, int x, int y) {
	switch (key) 
	{
	case GLUT_KEY_UP:

		currTarget = (currTarget + 1) % meshes.size();
		cout << currTarget;
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

