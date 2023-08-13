#include <stdio.h>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Headers/Shader/shader_stuff.h"
#include <iostream>
#include <glm.hpp>
#include <gtx/string_cast.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include "Headers/Mesh/obj_loader.h"
#include "Headers/Mesh/Mesh.h"
#include "Headers/Mesh/Vertex.h"
#include "Headers/Lighting/Lights.h"
#include "Headers/Variables/sceneObjects.h"
#include "Headers/Shader/Shader.h"
#include "Headers/Texture/TextureManager.h"
#include "Headers/Mesh/MeshGenerator.h"
#include "Headers/Lighting/LightManager.h"
#include "Headers/Input/InputHandler.h"

TextureManager texManager;
LightManager lightManager(firePositions.size());

int WIDTH = 1280;
int HEIGHT = 640;
std::vector<Mesh> meshes;
std::vector<Mesh> fireAccesiories;
std::vector<Mesh> emissions;
std::vector<Mesh> rotatingMeshes;
float objMove[2] = { 0.f, 3.14 };
glm::mat4 matProj;
glm::mat4 matView;

int frame;
int limit = 15 + 1;

Shader program;
Shader program_no_light;
Shader program_hdr;
Shader program_background;
Shader program_irradiance;
Shader program_prefilter;
Shader program_brdf;

glm::vec3 camPos;

const float PI = 3.1415926f;

bool Anisotropy = true;

void DisplayScene()
{
	frame++;
	frame = frame % limit;
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camPos = glm::vec3(sin(_scene_rotate_y) * dist, 4.0 + _scene_rotate_x * dist, cos(_scene_rotate_y) * dist);
	matView = glm::lookAt(camPos, glm::vec3(0.0, 4.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

	program.use();
	texManager.activateHdrTextures(program);
	lightManager.setPointStr(frame, limit, midMaxFunc);
	lightManager.enablePointLights(lightState);
	lightManager.setUniforms(program.getId());
	program.setVec3(camPos, "camPos");
	program.setMat4(matProj, "matProj");
	program.setMat4(matView, "matView");
	for (int i = 0; i < rotatingMeshes.size(); i++) 
	{
		rotatingMeshes[i].setTranslation(objMove[i], exampleTranslationFunc);
		objMove[i] += .01;
		if (objMove[i] > PI * 2)
			objMove[i] = 0;
	}
	program.display(meshes);
	program.display(fireAccesiories);
	program.display(rotatingMeshes);
	
	glEnable(GL_BLEND);
	program_no_light.use();
	program_no_light.setVec3(camPos, "camPos");
	program_no_light.setMat4(matProj, "matProj");
	program_no_light.setMat4(matView, "matView");
	program_no_light.display(emissions);
	glDisable(GL_BLEND);

	program_background.use();
	program_background.setMat4(matProj, "matProj");
	program_background.setMat4(matView, "matView");
	program_background.setInt(0, "hdrMap");
	texManager.activateHDR(program_background.getId());

	glUseProgram(0);
	glutSwapBuffers();
}

void Initialize()
{
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);



	if (glGetError() != GL_NO_ERROR)
	{
		printf("Error: GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT;\n");
		maxAnisotropy = -1.0;
	}
	else
	{
		printf("Max Anisotropy: %f\n", maxAnisotropy);
	}


	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	program_hdr = Shader("Shaders/Hdr/vertex_hdr.glsl", "Shaders/Hdr/fragment_hdr.glsl");
	program_irradiance = Shader("Shaders/Hdr/vertex_irradiance.glsl", "Shaders/Hdr/fragment_irradiance.glsl");	
	program_background = Shader("Shaders/Hdr/vertex_background.glsl", "Shaders/Hdr/fragment_background.glsl");
	program_prefilter = Shader("Shaders/Hdr/vertex_prefilter.glsl", "Shaders/Hdr/fragment_prefilter.glsl");
	program_brdf = Shader("Shaders/Hdr/vertex_brdf.glsl", "Shaders/Hdr/fragment_brdf.glsl");

	texManager.setupCompleteHDR(hdrPath, program_hdr, simpleMeshPtr("Objects/cube.obj"), program_irradiance, program_prefilter, program_brdf);

	program = Shader("Shaders/Pbr/vertex_default.glsl", "Shaders/Pbr/fragment_default.glsl");
	program_no_light = Shader("Shaders/misc/vertex_fire_animation.glsl", "Shaders/misc/fragment_fire_animation.glsl");
	
	lightManager.quickSetup(firePositions);
	lightManager.enablePointLights(true);
	for (int i = 0; i < OBJ_AMOUNT; i++)
	{
		generateMeshes(objNames[i], &meshes, texManager, Anisotropy, NULL, NULL,
			texNames[i * 5],
			texNames[i * 5 + 1],
			texNames[i * 5 + 2],
			texNames[i * 5 + 3],
			texNames[i * 5 + 4]
			);
	}

	for (int i = 0; i < EMISSION_OBJ_AMOUNT; i++)
	{
		generateMeshes(emissionObjNames[i], &emissions, texManager, Anisotropy, &firePositions, emissionTexNames[i], NULL, NULL, NULL, NULL, NULL, 15);

	}
	for (int i = 0; i < MULTIPLE_OBJ_AMOUNT; i++) 
	{
		generateMeshes(multipleObj[i], &fireAccesiories, texManager, Anisotropy, &firePositions, NULL,
			multipleObjTextures[i * 5],
			multipleObjTextures[i * 5 + 1],
			multipleObjTextures[i * 5 + 2],
			multipleObjTextures[i * 5 + 3],
			multipleObjTextures[i * 5 + 4]
		);
	}
	for (int x = 0; x < 2; x++) 
	{
		for (int i = 0; i < ROTATING_OBJ; i++) 
		{
			generateMeshes(rotatingObjNames[i], &rotatingMeshes, texManager, Anisotropy, NULL, NULL,
				rotatingTexNames[i * 5],
				rotatingTexNames[i * 5 + 1],
				rotatingTexNames[i * 5 + 2],
				rotatingTexNames[i * 5 + 3],
				rotatingTexNames[i * 5 + 4]
			);
		}
	}

	
	glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
	glBindVertexArray(0);

}


void Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	matProj = glm::perspectiveFov(glm::radians(90.0f), (float)width, (float)height, 0.1f, 100.f);
}

void animation(int keyframe)
{
	glutPostRedisplay();
	glutTimerFunc(1000/60, animation, 0);
}

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
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);
	glutMouseWheelFunc(mouseWheel);
	int podMenu = glutCreateMenu(Menu);
	glutAddMenuEntry("Light ON", 1);
	glutAddMenuEntry("Light OFF", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	/*glutSpecialFunc(SpecialKeys);*/
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(1000 / 60, animation, 0);
	glutMainLoop();

	// Cleaning();
	program.remove();
	program_no_light.remove();
	//for (Mesh m : meshes) {
	//	m.deallocateMemory();
	//}
	//for (Mesh m : transparentMeshes) {
	//	m.deallocateMemory();
	//}

	return 0;
}