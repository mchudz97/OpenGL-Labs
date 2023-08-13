#pragma once
#define __CHECK_FOR_ERRORS {GLenum errCode; if ((errCode = glGetError()) != GL_NO_ERROR) printf("Error (%d): %s in file %s at line %d !\n", errCode, gluErrorString(errCode), __FILE__, __LINE__);}
class Minimap 
{
private:
	GLuint FBO = 0;
	GLuint mapTexture = 0;
	GLuint VAO = 0;
	GLuint VBO = 0;
	GLuint RBO = 0;
	
	glm::vec2 wh;
	glm::mat4 projection;
	GLuint res;
	float height;
	GLuint idVAO = 0;
	GLuint idVBO_coord = 0;
	void setupFBO() 
	{
		if (!this->FBO)
			glGenFramebuffers(1, &this->FBO);
		if (!this->mapTexture)
			glGenTextures(1, &this->mapTexture);
		if (!this->RBO)
			glGenRenderbuffers(1, &this->RBO);

		
		glBindTexture(GL_TEXTURE_2D, this->mapTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->res, this->res, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glBindRenderbuffer(GL_RENDERBUFFER, this->RBO);
		glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
		
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, this->res, this->res);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->RBO);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->mapTexture, 0);
		

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			printf("Error: Framebuffer is not complete!\n");
			exit(1);
		}

		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

public:
	Minimap(){}
	Minimap(
		float height,
		GLuint resolution = 256,
		float w = 200.f,
		float h = 200.f
		)
	{
		float x = glutGet(GLUT_WINDOW_WIDTH) - w - 10;
		float y = glutGet(GLUT_WINDOW_HEIGHT) - h - 10;
		this->wh = glm::vec2(w, h);
		this->res = resolution;
		this->height = height;
		this->projection = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, .1f, this->height * 2);
		
		float vertices[6][4] = {
				{ x,     y,		  0.0f, 0.0f },
				{ x,     y + h,   0.0f, 1.0f },
				{ x + w, y,       1.0f, 0.0f },

				{ x,     y + h,   0.0f, 1.0f },
				{ x + w, y,       1.0f, 0.0f },
				{ x + w, y + h,   1.0f, 1.0f }
		};

		glGenVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);
		glGenBuffers(1, &this->VBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		this->setupFBO();
	}
	void updateMapPos() 
	{
		float x = glutGet(GLUT_WINDOW_WIDTH) - wh.x - 10;
		float y = glutGet(GLUT_WINDOW_HEIGHT) - wh.y - 10;
		float vertices[6][4] = {
		{ x,			y,		  0.0f, 0.0f },
		{ x,     y + wh.y,		  0.0f, 1.0f },
		{ x + wh.x,		y,		  1.0f, 0.0f },

		{ x,     y + wh.y,		  0.0f, 1.0f },
		{ x + wh.x,		y,		  1.0f, 0.0f },
		{ x + wh.x,		y + wh.y, 1.0f, 1.0f }
		};
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	

	void updateMap(void(*objectRenderingFunction)(Shader), glm::vec3 camPos,  Shader& mapShader)
	{
		glViewport(0, 0, this->res, this->res);
		glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
		mapShader.use();
		mapShader.setMat4(this->projection, "matProj");
		glm::mat4 v = glm::lookAt(glm::vec3(camPos[0], this->height, camPos[2]),
			glm::vec3(camPos[0], this->height - 1 , camPos[2]),
			glm::vec3(0, 0, 1));
		mapShader.setMat4(v, "matView");
		mapShader.setInt(this->res, "res");
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		objectRenderingFunction(mapShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}
	void renderToScene(Shader& shader) 
	{
		updateMapPos();
		shader.use();
		glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(glutGet(GLUT_WINDOW_WIDTH)), 0.0f, static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT)));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->mapTexture);
		shader.setInt(0, "minimap");
		shader.setMat4(proj, "matProj");
		glBindVertexArray(this->VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

	}
};