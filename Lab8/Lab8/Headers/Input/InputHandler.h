#pragma once

GLfloat _scene_rotate_x = 0.0f;
GLfloat _scene_rotate_y = 0.0f;
GLfloat _scene_translate_x = 0.0f;
int _mouse_buttonState = GLUT_UP;
int _mouse_buttonX, _mouse_buttonY;
float dist = 10;
bool lightState = true;
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
        _scene_rotate_y +=  (x - _mouse_buttonX) / (float)glutGet(GLUT_WINDOW_WIDTH);
        _mouse_buttonX = x;
        _scene_rotate_x +=  (y - _mouse_buttonY) / (float)glutGet(GLUT_WINDOW_HEIGHT);
        _mouse_buttonY = y;
        glutPostRedisplay();
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
		break;

	case 'd':
		break;

	case 'w':
		break;

	case 's':
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
	default:
		return;
	}

}