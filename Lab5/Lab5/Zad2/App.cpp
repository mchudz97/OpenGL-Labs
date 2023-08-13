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

// -----------------------------------------------
// NOWE: funkcja do wczytywania plikow graficznych
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// -----------------------------------------------

int WIDTH = 640;
int HEIGHT = 480;


// ---------------------------------------
GLuint idProgram;
GLuint vBuffer_pos;
GLuint vBuffer_color;
GLuint vBuffer_uv;
GLuint vArray;
GLuint idTexture1;

// ---------------------------------------
glm::mat4x4 matProj;
glm::mat4x4 matView;
glm::mat4x4 matProjView;
glm::vec3 vTrans;
glm::vec3 vRot;

float uvPos = 0.f;

// ---------------------------------------
GLfloat vertices_pos[] =
{
	-1.0f, -1.0f, 0.0f,	// bottom left
	 1.0f, -1.0f, 0.0f,	// bottom right
	 1.0f,  1.0f, 0.0f, // upper right
	 -1.0f, 1.0f, 0.0f,	// upper left
	 1.0f, 1.0f, 0.0f,	// upper right
	 -1.0f, -1.0f, 0.0f, // bottom left
};


GLfloat vertices_tex[] =
{
	0.3f, 0.0f, // for bottom left vertex
	0.7f, 0.0f, // for bottom right vertex
	0.7f, 1.0f, // for upper right vertex 
	0.3f, 1.0f, // for upper left vertex
	0.7f, 1.0f, // for upper right vertex
	0.3f, 0.0f, // for bottom left vertex
};



// ---------------------------------------
void DisplayScene()
{

	// Czyszczenie ramki (jak zwykle)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	matView = glm::mat4x4(1.0);
	matView = glm::translate(matView, vTrans);
	matView = glm::rotate(matView, vRot.x, glm::vec3(1.0f, 0.0f, 0.0f));
	matView = glm::rotate(matView, vRot.y, glm::vec3(0.0f, 1.0f, 0.0f));

	matProjView = matProj * matView;

	glUseProgram(idProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, idTexture1);
	glUniform1i(glGetUniformLocation(idProgram, "tex1"), 0);

	glUniform1f(glGetUniformLocation(idProgram, "uvPos"), uvPos);

	std::cout << uvPos << "\n";
	glBindVertexArray(vArray);
	
	glUniformMatrix4fv(glGetUniformLocation(idProgram, "matProjView"), 1, GL_FALSE, glm::value_ptr(matProjView));


	// Generowanie obiektow na ekranie (jak zwykle)
	glDrawArrays(GL_TRIANGLES, 0, 3 * 3 * 2);


	glUseProgram(0);
	glBindVertexArray(0);



	glutSwapBuffers();
}





// ---------------------------------------------------
void CreateTexture(const char *path, GLuint &idTex)
{

	// -------------------------------------------------------
	// 1. Wczytanie pliku graficznego
	// -------------------------------------------------------

	 int tex_width, tex_height, n;
	 unsigned char *tex_data;
	 stbi_set_flip_vertically_on_load(true); // tylko raz aby ...

	 tex_data = stbi_load(path, &tex_width, &tex_height, &n, 0);
	 if (tex_data == NULL) 
	 {
	 	printf("Image can't be loaded!\n");
	 	exit(1);
	 }

	// -------------------------------------------------------
	// 2. Generowanie identyfikatora i ustawianie jako aktualnej tekstury
	// -------------------------------------------------------

	 glGenTextures(1, &idTex);
	 glBindTexture(GL_TEXTURE_2D, idTex);

	// -------------------------------------------------------
	// 3. Przekazanie danych tekstury
	// -------------------------------------------------------
	 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
	 stbi_image_free(tex_data);

	// -------------------------------------------------------
	// 4. Ustawianie parametrow tekstury
	// -------------------------------------------------------

	// (a) Zawijanie tekstury

	// GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Ustalenie koloru ramki w przypadku wybrania GL_CLAMP_TO_BORDER
	// float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	// glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

	// (b) Filtr powiekszenia

	// GL_LINEAR, GL_NEAREST
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// (c) Filtr pomniejszenia

	// GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST,
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);


	// -------------------------------------------------------
	// 4. Gdy wybrano korzystanie z MIPMAP
	// -------------------------------------------------------

	// Generowanie mipmap automatycznie
	glGenerateMipmap(GL_TEXTURE_2D);

	// Podstawowy level mipmap (default 0)
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
}

// ---------------------------------------------------
void Initialize()
{


	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	// Programowanie potoku
	idProgram = glCreateProgram();

	glAttachShader(idProgram, LoadShader(GL_VERTEX_SHADER, "vertex.glsl"));
	glAttachShader(idProgram, LoadShader(GL_FRAGMENT_SHADER, "fragment.glsl"));

	LinkAndValidateProgram(idProgram);

	// VAO
	glGenVertexArrays(1, &vArray);
	glBindVertexArray(vArray);

	// Wspolrzedne wierzchokow
	glGenBuffers(1, &vBuffer_pos);
	glBindBuffer(GL_ARRAY_BUFFER, vBuffer_pos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_pos), vertices_pos, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	

	// Wspolrzedne textury UV
	glGenBuffers(1, &vBuffer_uv);
	glBindBuffer(GL_ARRAY_BUFFER, vBuffer_uv);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_tex), vertices_tex, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	CreateTexture("background.png", idTexture1);


	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(0);

}


// ---------------------------------------
void Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	matProj = glm::perspectiveFov(glm::radians(60.0f), (float)width, (float)height, 0.1f, 100.f);
}
void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:	// ESC key
		exit(0);
		break;

	case 'a':
		uvPos -= .01f;
		glutPostRedisplay();
		break;

	case 'd':
		uvPos += .01f;
		glutPostRedisplay();
		break;
	}

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
	glutMotionFunc(MouseMotion);
	glutMouseWheelFunc(MouseWheel);
	
	glutSpecialFunc(SpecialKeys);*/
	glutKeyboardFunc(Keyboard);
	glutMainLoop();

	// Cleaning();
	glDeleteProgram(idProgram);
	glDeleteBuffers(1, &vBuffer_pos);
	glDeleteBuffers(1, &vBuffer_uv);
	glDeleteBuffers(1, &vBuffer_color);
	glDeleteVertexArrays(1, &vArray);

	return 0;
}