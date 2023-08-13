#pragma once
#include "Headers/Time/DeltaTime.h"
GLfloat _scene_rotate_x = 0.0f;
GLfloat _scene_rotate_y = 0.0f;
GLfloat _scene_translate_x = 0.0f;
int _mouse_buttonState = GLUT_UP;
int _mouse_buttonX, _mouse_buttonY;
float dist = 10;
bool lightState = true;
bool irradianceState = true;
GLuint shadowRes = 512;
bool dirShadowEnabled = false;
bool pointShadowEnabled = false;
DeltaTime delta;
double defaultDelta = 1000 / 120;
float tick = 0.01;
bool freeMode = false;
GLfloat yaw = 0.f;
GLfloat pitch = 0.f;
float b1 = 0.0001;
float b2 = 0.00005;
glm::vec3 freeCamPos = glm::vec3(11.f, 2.f, 0.f);
glm::vec3 freeCamFront = glm::vec3(1.f, 0.f, 0.f);
void MouseButton(int button, int state, int x, int y)
{

    if (button == GLUT_LEFT_BUTTON)
    {
        _mouse_buttonState = state;

        if (state == GLUT_DOWN)
        {
            _mouse_buttonX = x;
            _mouse_buttonY = y;
        }
    }
}



void MouseMotion(int x, int y)
{

    if (_mouse_buttonState == GLUT_DOWN)
    {
		float h = (x - _mouse_buttonX) / (float)glutGet(GLUT_WINDOW_WIDTH) * delta.getDelta() / defaultDelta;
		float v = (y - _mouse_buttonY) / (float)glutGet(GLUT_WINDOW_HEIGHT) * delta.getDelta() / defaultDelta;
		if (!freeMode) 
		{
			_scene_rotate_y += h;
			_scene_rotate_x += v;
		}
		else 
		{
			yaw += h * 100;
			pitch += v * 100;

			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;

			glm::vec3 front;
			front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			front.y = sin(glm::radians(pitch));
			front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
			freeCamFront = glm::normalize(front);
		}
        _mouse_buttonX = x;
        _mouse_buttonY = y;
    }
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:	// ESC key
		exit(0);
		break;

	case 'a':
		if (freeMode)
		freeCamPos -= 0.1f * glm::normalize(glm::cross(freeCamFront, glm::vec3(0, 1, 0)));
		break;

	case 'd':
		if (freeMode)
		freeCamPos += 0.1f * glm::normalize(glm::cross(freeCamFront, glm::vec3(0, 1, 0)));
		break;

	case 'w':
		if (freeMode)
			freeCamPos += 0.1f * freeCamFront;
		break;

	case 's':
		if (freeMode)
			freeCamPos -= 0.05f * freeCamFront;
		break;
	default:
		return;
	}
}

void mouseWheel(int button, int dir, int x, int y)
{
	if (!freeMode) {
		if (dir > 0)
			dist -= .5f * delta.getDelta() / defaultDelta;
		else
			dist += .5f * delta.getDelta() / defaultDelta;
	}
}

void Menu(int option) 
{

	switch (option)
	{
	case 1:
		lightState = true;
		break;
	case 2:
		lightState = false;
		break;
	case 3:
		freeMode = true;
		break;
	case 4:
		freeMode = false;
		break;
	case 5:
		tick = 0.01f;
		break;
	case 6:
		tick = 0.f;
		break;
	case 7:
		irradianceState = true;
		break;
	case 8:
		irradianceState = false;
		break;
	case 9:
		shadowRes = 512;
		break;
	case 10:
		shadowRes = 1024;
		break;
	case 11:
		shadowRes = 2048;
		break;
	case 12:
		pointShadowEnabled = true;
		break;
	case 13:
		pointShadowEnabled = false;
		break;
	case 14:
		dirShadowEnabled = true;
		break;
	case 15:
		dirShadowEnabled = false;
		break;
	default:
		return;
	}

}

void setupMenu() 
{
	int lightControl = glutCreateMenu(Menu);
	glutAddMenuEntry("On", 1);
	glutAddMenuEntry("Off", 2);
	int camera = glutCreateMenu(Menu);
	glutAddMenuEntry("Free", 3);
	glutAddMenuEntry("Perspective", 4);
	int rotationControl = glutCreateMenu(Menu);
	glutAddMenuEntry("Resume", 5);
	glutAddMenuEntry("Stop", 6);
	int irradianceControl = glutCreateMenu(Menu);
	glutAddMenuEntry("On", 7);
	glutAddMenuEntry("Off", 8);
	int shadowResolution = glutCreateMenu(Menu);
	glutAddMenuEntry("512", 9);
	glutAddMenuEntry("1024", 10);
	glutAddMenuEntry("2048", 11);
	int pointShadows = glutCreateMenu(Menu);
	glutAddMenuEntry("On", 12);
	glutAddMenuEntry("Off", 13);
	int dirShadows = glutCreateMenu(Menu);
	glutAddMenuEntry("On", 14);
	glutAddMenuEntry("Off", 15);
	glutCreateMenu(Menu);
	glutAddSubMenu("Lights", lightControl);
	glutAddSubMenu("Irradiance", irradianceControl);
	glutAddSubMenu("Shadow resolution", shadowResolution);
	glutAddSubMenu("Camera Mode", camera);
	glutAddSubMenu("Moving meshes", rotationControl);
	glutAddSubMenu("point shadows", pointShadows);
	glutAddSubMenu("directional shadows", dirShadows);


	glutAttachMenu(GLUT_RIGHT_BUTTON);
}