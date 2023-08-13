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
#include "Headers/Helper/HelperFunctions.h"
#include "Headers/Mesh/obj_loader.h"
#include "Headers/Lighting/Lights.h"
#include "Headers/Mesh/Vertex.h"
#include "Headers/Shader/Shader.h"
#include "Headers/Lighting/LightManager.h"
#include "Headers/Text/CFont.h"
#include "Headers/Mesh/BaseMesh.h"
#include "Headers/Mesh/InstantableMesh.h"
#include "Headers/Mesh/Mesh.h"
#include "Headers/Mesh/NamedMesh.h"
#include "Headers/Texture/TextureManager.h"
#include "Headers/Variables/sceneObjects.h"
#include "Headers/Mesh/MeshGenerator.h"
#include "Headers/Input/InputHandler.h"
#include "Headers/Time/DeltaTime.h"
#include "Headers/Mesh/Model.h"
#include "Headers/Helper/Shortcuts.h"
#include "Headers/Particles/ParticleGenerator.h"
#include "Headers/Shadows/Shadows.h"
#include "Headers/Map/Minimap.h"


extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

TextureManager texManager;
LightManager lightManager(firePositions.size());

double actualDelta = 0.0;
int WIDTH = 1280;
int HEIGHT = 640;
std::vector<PointLightShadow> pointLightShadows;
DirLightShadow dls;
std::vector<Mesh> meshes;
std::vector<InstantableMesh> fireAccesiories;
std::vector<InstantableMesh> flames;
std::vector<ParticleGenerator> smokes;
std::vector<NamedMesh> rotatingNamedMeshes;
std::vector<CFont> fonts;

float objMove[2] = { 0.f, 3.14 };
glm::mat4 matProj;
glm::mat4 matView;
int frame;
int limit = 15 + 1;

Shader program;
Shader program_instanced;
Shader program_no_light;
Shader program_hdr;
Shader program_background;
Shader program_irradiance;
Shader program_prefilter;
Shader program_brdf;
Shader text_3d;
Shader program_smoke;
Shader program_shadow;
Shader program_shadowDir;
Shader program_shadowDirSmoke;
Shader program_mapRender;
Shader program_mapDisplay;

glm::vec3 dirLightPos = glm::vec3(10, 15, -8);
glm::vec3 camPos;

const float PI = 3.14159265359f;

bool Anisotropy = true;

CFont c1;
CFont c2(false);
CFont c3(false);

Minimap minimap;

template <class T>
void display(T &meshVec, Shader s, bool tex = true) 
{
	s.setBool(tex, "irradianceEnabled");
	if (tex) 
		texManager.activateHdrTextures(s);

	for (BaseMesh& m : meshVec)
		m.render(s);
}

void display(std::vector<ParticleGenerator>& particles, Shader s, glm::vec3 cPos, bool tex = true)
{
	if (tex)
		texManager.activateHdrTextures(s);
	std::sort(particles.begin(), particles.end(), [cPos](ParticleGenerator f, ParticleGenerator s)
		{
			GLfloat d1 = glm::distance(cPos, f.getPosition());
			GLfloat d2 = glm::distance(cPos, s.getPosition());
			return d1 > d2;
		});
	for (ParticleGenerator& p : particles)
		p.render(s, cPos);

}

void displaySmoke(Shader s, glm::vec3 cPos)
{
	display(smokes, s, cPos, false);
}
template <class T>
void displayToShadowShader(T& meshVec, Shader s) 
{
	for (BaseMesh& m : meshVec)
		m.shadowRender(s);
}

template <class T>
void displayToMapShader(T& meshVec, Shader s)
{
	for (BaseMesh& m : meshVec)
		m.renderToMap(s);
}

void renderToDeepMap(Shader s) {
	displayToShadowShader(meshes, s);
	displayToShadowShader(fireAccesiories, s);
	displayToShadowShader(rotatingNamedMeshes, s);

}
void renderToMapShader(Shader s) 
{
	displayToMapShader(meshes, s);
	displayToMapShader(fireAccesiories, s);
	displayToMapShader(rotatingNamedMeshes, s);
}
void createDirLightShadowMap(Shader s) 
{
	if (!dirShadowEnabled)
		return;
	glEnable(GL_CULL_FACE);
	s.use();
	dls.render(s, renderToDeepMap, displaySmoke, program_shadowDirSmoke);
	glDisable(GL_CULL_FACE);
}
void createPointLightShadowMaps(Shader s) 
{
	if (!pointShadowEnabled)
		return;
	glEnable(GL_CULL_FACE);
	s.use();
	for (int i = 0; i < pointLightShadows.size(); i++) 
	{
		pointLightShadows[i].render(s, renderToDeepMap);
	}
	//glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
}
void attachShadowMaps(Shader s) 
{
	if (!pointShadowEnabled)
	{
		s.setBool(pointShadowEnabled, "pointShadowEnabled");
		return;
	}
	for (int i = 0; i < pointLightShadows.size(); i++)
		pointLightShadows[i].attachDepthmap(s, i, 13 + i);
}



void DisplayScene()
{
	delta.update();
	actualDelta = delta.getDelta();
	frame++;
	frame = frame % limit;
	
	lightManager.setPointStr(frame, limit, midMaxFunc);
	lightManager.enablePointLights(lightState);

	//dls.updateFarPlane(farPlane);

	

	if (shadowRes != SHADOW_RES)
		SHADOW_RES = shadowRes;
	
	if (tick) 
	{
		createDirLightShadowMap(program_shadowDir);
		createPointLightShadowMaps(program_shadow);
	}

	
	if (!freeMode) 
	{
		camPos = glm::vec3(sin(_scene_rotate_y) * dist, 4.0 + _scene_rotate_x * dist, cos(_scene_rotate_y) * dist);
		matView = glm::lookAt(camPos, glm::vec3(0, 4, 0), glm::vec3(0.0, 1.0, 0.0));
	}
	else {
		camPos = freeCamPos;
		matView = glm::lookAt(freeCamPos, freeCamPos + freeCamFront, glm::vec3(0, 1,0));
	}

	minimap.updateMap(renderToMapShader, camPos, program_mapRender);
	
	glViewport(0, 0, WIDTH, HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	program.use();

	lightManager.setUniforms(program);
	setVP(program, matView, matProj, camPos);
	for (int i = 0; i < rotatingNamedMeshes.size(); i++) 
	{
		rotatingNamedMeshes[i].setTranslation(objMove[i], exampleTranslationFunc);
		objMove[i] += tick * delta.getDelta() / defaultDelta;
		if (objMove[i] > PI * 1'000'000)
			objMove[i] = 0;
	}
	attachShadowMaps(program);
	program.setBool(dirShadowEnabled, "dirShadowEnabled");
	if (dirShadowEnabled)
		dls.attachDepthmap(program);

	
	display(meshes, program, irradianceState);
	display(rotatingNamedMeshes, program, irradianceState);

	program_instanced.use();
	attachShadowMaps(program_instanced);
	program_instanced.setBool(dirShadowEnabled, "dirShadowEnabled");
	if(dirShadowEnabled)
		dls.attachDepthmap(program_instanced);
	lightManager.setUniforms(program_instanced);
	setVP(program_instanced, matView, matProj, camPos);

	display(fireAccesiories, program_instanced, irradianceState);
	
	glEnable(GL_BLEND);
	program_no_light.use();
	setVP(program_no_light, matView, matProj, camPos);
	display(flames, program_no_light, false);
	program_smoke.use();
	setVP(program_smoke, matView, matProj);
	display(smokes, program_smoke, camPos, false);
	glDisable(GL_BLEND);
	
	program_background.use();
	program_background.setMat4(matProj, "matProj");
	program_background.setMat4(matView, "matView");
	
	texManager.activateHDR(program_background);

	for (int i = 0; i < rotatingNamedMeshes.size(); i++) {
		rotatingNamedMeshes[i].renderText(matView, matProj);
	}

	if(delta.getFPS() >= 56)
		c1.renderText(delta.fpsInfo(), 25, 25, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	else if(delta.getFPS() >= 35)
		c1.renderText(delta.fpsInfo(), 25, 25, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));
	else
		c1.renderText(delta.fpsInfo(), 25, 25, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	//glEnable(GL_BLEND);
	minimap.renderToScene(program_mapDisplay);
	//glDisable(GL_BLEND);
	glUseProgram(0);
	glutSwapBuffers();
}

void Initialize()
{
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);

	c1.init("Fonts/INFECTED.ttf", 36);
	
	text_3d = Shader("Shaders/Text/vertex_text_scene.glsl", "Shaders/Text/text-ft-fragment.glsl");
	c2.init("Fonts/comicsans.ttf", 36, text_3d);

	
	program_mapRender = Shader("Shaders/Map/vertex_map_renderer.glsl", "Shaders/Map/fragment_map_renderer.glsl");
	program_mapDisplay = Shader("Shaders/Map/vertex_display_map.glsl", "Shaders/Map/fragment_display_map.glsl");

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
	glCullFace(GL_BACK);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	
	minimap = Minimap(10);

	program_hdr = Shader("Shaders/Hdr/vertex_hdr.glsl", "Shaders/Hdr/fragment_hdr.glsl");
	program_irradiance = Shader("Shaders/Hdr/vertex_irradiance.glsl", "Shaders/Hdr/fragment_irradiance.glsl");	
	program_background = Shader("Shaders/Hdr/vertex_background.glsl", "Shaders/Hdr/fragment_background.glsl");
	program_prefilter = Shader("Shaders/Hdr/vertex_prefilter.glsl", "Shaders/Hdr/fragment_prefilter.glsl");
	program_brdf = Shader("Shaders/Hdr/vertex_brdf.glsl", "Shaders/Hdr/fragment_brdf.glsl");

	texManager.setupCompleteHDR(hdrPath, program_hdr, simpleMeshPtr("Objects/cube.obj"), program_irradiance, program_prefilter, program_brdf);

	program = Shader("Shaders/Pbr/vertex_default.glsl", "Shaders/Pbr/fragment_default.glsl");
	program_instanced = Shader("Shaders/Pbr/vertex_instanced.glsl", "Shaders/Pbr/fragment_default.glsl");
	program_instanced.use();
	program_no_light = Shader("Shaders/misc/vertex_fire_animation.glsl", "Shaders/misc/fragment_fire_animation.glsl");
	program_smoke = Shader("Shaders/Particles/vertex_smoke.glsl", "Shaders/Particles/fragment_smoke.glsl");
	program_shadow = Shader("Shaders/Shadows/vertex_point_shadow.glsl", "Shaders/Shadows/fragment_point_shadow.glsl", "Shaders/Shadows/geometry_point_shadow.glsl");
	program_shadowDir = Shader("Shaders/Shadows/vertex_directional.glsl", "Shaders/Shadows/fragment_directional.glsl");
	program_shadowDirSmoke = Shader("Shaders/Shadows/vertex_directional_sm.glsl", "Shaders/Shadows/fragment_directional_sm.glsl");

	dls = DirLightShadow(dirLightPos);
	lightManager.quickSetup(firePositions);
	lightManager.enablePointLights(true);
	lightManager.enableDirLight(true);
	lightManager.setDirLight(dirLightPos, glm::vec3(.8f, .3f, .8f));

	for (int i = 0; i < OBJ_AMOUNT; i++)
	{
		generateMeshes(objNames[i], meshes, texManager, Anisotropy, NULL, NULL,
			texNames[i * 5],
			texNames[i * 5 + 1],
			texNames[i * 5 + 2],
			texNames[i * 5 + 3],
			texNames[i * 5 + 4]
			);
	}

	for (int i = 0; i < EMISSION_OBJ_AMOUNT; i++)
	{
		generateInstancedMesh(emissionObjNames[i], flames, texManager, Anisotropy, firePositions, emissionTexNames[i], NULL, NULL, NULL, NULL, NULL, 15);

	}
	for (int i = 0; i < MULTIPLE_OBJ_AMOUNT; i++) 
	{
		generateInstancedMesh(multipleObj[i], fireAccesiories, texManager, Anisotropy, firePositions, NULL,
			multipleObjTextures[i * 5],
			multipleObjTextures[i * 5 + 1],
			multipleObjTextures[i * 5 + 2],
			multipleObjTextures[i * 5 + 3],
			multipleObjTextures[i * 5 + 4]
		);
	}
	for (int i = 0; i < firePositions.size(); i++) {
		std::vector<InstantableMesh> tmp;
		std::vector<glm::vec3> empty;
		generateInstancedMesh("Objects/spr.obj", tmp, texManager, Anisotropy, empty, "Textures/smoke.png");
		smokes.push_back(ParticleGenerator(tmp[0], firePositions[i], .25f, 4, 1, .05f, .01f, .3f));
	}
	for (int i = 0; i < firePositions.size(); i++) 
	{
		pointLightShadows.push_back(PointLightShadow(firePositions[i]));
	}
	for (int i = 0; i < names.size(); i++) 
	{
		generateNamedMesh(c2, 2, names[i], rotatingObjNames[0], rotatingNamedMeshes, texManager, Anisotropy, NULL,
			rotatingTexNames[0],
			rotatingTexNames[1],
			rotatingTexNames[2],
			rotatingTexNames[3],
			rotatingTexNames[4]
		);
	}

	/*rotatingNamedMeshes.push_back(NamedMesh(rotatingMeshes[0], c2, 2, "kulku"));
	rotatingNamedMeshes.push_back(NamedMesh(rotatingMeshes[1], c2, 2, "kulku 2"));*/
	glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
	glBindVertexArray(0);

	delta.init();
}


void Reshape(int width, int height)
{
	WIDTH = width;
	HEIGHT = height;
	matProj = glm::perspectiveFov(glm::radians(90.0f), (float)width, (float)height, 0.1f, 100.f);
	c1.updateProjection2D();
}

void animation(int keyframe)
{
	glutPostRedisplay();
	glutTimerFunc(1000/120, animation, 0);
}

int main(int argc, char* argv[])
{
	// GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("OpenGL10");

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
	
	setupMenu();

	/*glutSpecialFunc(SpecialKeys);*/
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(1000 / 120, animation, 0);
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