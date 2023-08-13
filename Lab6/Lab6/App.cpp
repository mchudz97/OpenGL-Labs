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
#include "Vertex.h"
#include "MeshGenerator.h"
#include "sceneObjects.h"
#include "Texture.h"
#include "Shader.h"
#include "DepthMap.h"

#define __CHECK_FOR_ERRORS 	{GLenum errCode; if ((errCode = glGetError()) != GL_NO_ERROR) printf("Error (%d): %s in file %s at line %d !\n", errCode, gluErrorString(errCode), __FILE__,  __LINE__);}
int WIDTH = 640;
int HEIGHT = 480;
int objAmount = OBJ_AMOUNT;

std::vector<Mesh> meshes;
std::vector<Mesh> staticMeshes1;
std::vector<Mesh> emissions1;
std::vector<Mesh> transparentMeshes;
glm::mat4 matProj;
glm::mat4 matView;
float rot = 10;
float rotY = 0;
float dist = 10;

Shader program;
Shader program_no_light;
Shader program_depth;
std::vector<DepthMap> maps;


unsigned p, k;
int frame;
int limit = 15;
glm::vec3 camPos;

void DisplayScene()
{
	frame++;
	frame = frame % limit;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	camPos = glm::vec3(sin(rot) * dist, rotY * dist, cos(rot) * dist);
	matView = glm::lookAt(camPos, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	program_depth.use();
	for (int i = 0; i < firePositions.size(); i++) 
	{
		
		maps[i].render(program_depth, firePositions[i]);
		program_depth.display(meshes);
		maps[i].unbind();
	}
	
	
	
	program.use();
	for (int i = 0; i < maps.size(); i++) {
		maps[i].sendMap(program.getId(), i);
	}
	program.setInt(1, "depthMap");
	program.setFloat(25, "far_plane");
	program.setVec3(camPos, "camPos");
	program.setMat4(matProj, "matProj");
	program.setMat4(matView, "matView");
	
	if(p % 2 == 0)
		setFireUniforms(program.getId(), frame, limit);
	else
		setFireUniforms(program.getId(), frame, limit, true);
	if(k % 2 == 0)
		setDirectionLight(program.getId());
	else
		setDirectionLight(program.getId(), true);
	program.display(meshes);
	program.display(staticMeshes1, firePositions);
	
	program_no_light.use();
	program_no_light.setVec3(camPos, "camPos");
	program_no_light.setMat4(matProj, "matProj");
	program_no_light.setMat4(matView, "matView");
	program_no_light.display(emissions1);
	glUseProgram(0);
	glutSwapBuffers();
}

void Initialize()
{
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_CULL_FACE);
	program = Shader("vertex_default.glsl", "fragment_default.glsl");
	program_no_light = Shader("vertex_no_light.glsl", "fragment_no_light.glsl");
	program_depth = Shader("vertex_depth.glsl", "fragment_depth.glsl", "geometry_depth.glsl");
	
	for (int i = 0; i < OBJ_AMOUNT; i++) 
	{
		generateMeshes(objNames[i], &meshes, NULL, NULL,  texNames[i * 3], texNames[i * 3 + 1], texNames[i * 3 + 2], true);
	}
	for (int i = 0; i < ST_OBJ_AMOUNT; i++) 
	{
		generateMeshes(staticObjNames[i], &staticMeshes1, NULL, NULL, staticTexNames[i * 3], staticTexNames[i * 3 + 1], staticTexNames[i * 3 + 2], true);
	}
	for (int i = 0; i < EMISSION_OBJ_AMOUNT; i++)
	{
		generateMeshes(emissionObjNames[i], &emissions1, &firePositions, emissionTexNames[i], NULL, NULL, NULL, false, true, 15);
		
	}
	for (int i = 0; i < firePositions.size(); i++) {
		maps.push_back(DepthMap());
		maps[i].setup();
	}
	glViewport(0, 0, WIDTH, HEIGHT);
	glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);

	glDisable(GL_CULL_FACE);
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
		rot -= .05f;
		break;

	case 'd':
		rot += .05f;
		break;

	case 'w':
		rotY += .05f;
		break;

	case 's':
		rotY -= .05f;
		break;

	case 'p':
		p++;
		break;

	case 'k':
		k++;
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
	glutTimerFunc(1000 / 60, animation, 0);
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
	glutCreateWindow("OpenGL6");

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