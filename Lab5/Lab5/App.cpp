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
#include "obj_loader.h"
#include "Mesh.h"
#include "vertex.h"
#include "MeshGenerator.h"
#include "sceneObjects.h"
#include "Texture.h"
#include <map>
#include "Shader.h"

int WIDTH = 640;
int HEIGHT = 480;
int objAmount = OBJ_AMOUNT;

std::vector<Mesh> meshes;
std::vector<Mesh> transparentMeshes;
glm::mat4 matProj;
glm::mat4 matView;
float rot = 0;
float rotY = 0;
float dist = 10;

Shader program;



void DisplayScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	matView = glm::mat4(1.0);
	matView = glm::lookAt(glm::vec3(sin(rot) * dist,
		rotY * dist,
		cos(rot) * dist),
		glm::vec3(0.0, 0.0, 0.0),
		glm::vec3(0.0, 1.0, 0.0)
	);
	glm::vec3 camPos = glm::vec3(matView[3]);

	program.use();
	program.setVec3(camPos, "camPos");
	program.setMat4(matProj, "matProj");
	program.setMat4(matView, "matView");
	setLightUniforms(program.getId());
	program.display(meshes);

	glUseProgram(0);
	glutSwapBuffers();
}


void Initialize()
{


	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);


	program = Shader("vertex.glsl", "fragment.glsl");
	generateMeshes(objNames[0], &meshes, NULL, texNames[0]);
	generateMeshes(objNames[1], &meshes, &lampPositions, texNames[1]);
	generateMeshes(objNames[2], &meshes, &lampPositions, texNames[2]);
	generateMeshes(objNames[3], &meshes, NULL, texNames[3]);
	generateMeshes(objNames[4], &meshes, NULL, texNames[4]);
	generateMeshes(objNames[5], &meshes, NULL, texNames[5]);
	generateMeshes(objNames[6], &meshes, &firePositions, texNames[6], 15, true);
	std::vector<glm::vec3> line = createLineOfPositions(glm::vec3(-8.01, .65, -2), 16, 2, glm::vec3(1, 0, 0));
	generateMeshes(objNames[6], &meshes, &line, texNames[6], 15, true);

	glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
	
	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(0);

}


// ---------------------------------------
void Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	matProj = glm::perspectiveFov(glm::radians(90.0f), (float)width, (float)height, 0.1f, 100.f);
}
void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:	// ESC key
		exit(0);
		break;

	case 'a':
		rot -= .1f;
		
		break;

	case 'd':
		rot += .1f;
		
		break;
	case 'w':
		rotY += .1f;
		
		break;
	case 's':
		rotY -= .1f;
		
		break;
	}

}
void mouseWheel(int button, int dir, int x, int y)
{

	if (dir > 0)
		dist -= .5f;
	else
		dist += .5f;
	
}
void animation(int keyframe)
{
	glutPostRedisplay();
	glutTimerFunc(1000/60, animation, 0);
}

// ---------------------------------------------------
int main(int argc, char* argv[])
{
	// GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("OpenGL");

	// GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		printf("GLEW Error\n");
		exit(1);
	}

	// OpenGL
	if (!GLEW_VERSION_3_3)
	{
		printf("Brak OpenGL 3.3!\n");
		exit(1);
	}


	Initialize();
	glutDisplayFunc(DisplayScene);
	glutReshapeFunc(Reshape);
	/*glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);*/
	glutMouseWheelFunc(mouseWheel);
	
	/*glutSpecialFunc(SpecialKeys);*/
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(1000 / 60, animation, 0);
	glutMainLoop();

	// Cleaning();
	program.remove();
	for (Mesh m : meshes) {
		m.deallocateMemory();
	}
	for (Mesh m : transparentMeshes) {
		m.deallocateMemory();
	}

	return 0;
}