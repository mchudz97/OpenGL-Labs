#pragma once
class DepthMap {
private:
	const unsigned SHADOW_WIDTH = 1024;
	const unsigned SHADOW_HEIGHT = 1024;
	GLuint FBO;
	GLuint depthCubemap;


public:
	
	DepthMap(){}
	void setup()
	{
		glGenFramebuffers(1, &FBO);
		glGenTextures(1, &depthCubemap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
		for (int i = 0; i < 6; ++i) 
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_DEPTH_COMPONENT,
				SHADOW_WIDTH,
				SHADOW_HEIGHT,
				0,
				GL_DEPTH_COMPONENT,
				GL_FLOAT,
				NULL);

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void render(Shader &shader, glm::vec3 lightPos) 
	{
		glViewport(0, 0, this->SHADOW_WIDTH, this->SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		float aspect = (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT;
		float n = 1.0f;
		float f = 25.0f;
		glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, n, f);

		std::vector<glm::mat4> shadowTransforms;
			shadowTransforms.push_back(shadowProj *
				glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
			shadowTransforms.push_back(shadowProj *
				glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
			shadowTransforms.push_back(shadowProj *
				glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
			shadowTransforms.push_back(shadowProj *
				glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
			shadowTransforms.push_back(shadowProj *
				glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
			shadowTransforms.push_back(shadowProj *
				glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
		
		
		shader.use();
		for (int i = 0; i < 6 ; ++i)
		{
			shader.setMat4(shadowTransforms[i], ("shadowMatrices[" + to_string(i) + "]").c_str());
		}
		shader.setFloat(f, "far_plane");
		
		shader.setVec3(lightPos, "lightPos");
		
	}
	void sendMap(GLuint idProgram, GLuint index) {
		glActiveTexture(GL_TEXTURE10 + index);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
		glUniform1i(glGetUniformLocation(idProgram, ("shadowMap[" + to_string(index) + "]").c_str()), 10 + index);
		glActiveTexture(0);
	}
	void unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
};