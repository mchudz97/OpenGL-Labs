#pragma once
static float SHADOW_RES = 512;

class DirLightShadow 
{
private:
	GLuint depthFBO = 0;
	GLuint depthmap = 0;
	glm::vec3 direction;
	glm::mat4 shadowProj;
	glm::mat4 shadowView;
	glm::vec3 position;
	float near_plane = 1.0f;
	float far_plane = 60.f;
	float actualRes;
	bool enabled;
	void setupFBO() 
	{
		if (!this->depthFBO)
			glGenFramebuffers(1, &this->depthFBO);

		if (!this->depthmap)
			glGenTextures(1, &this->depthmap);

		glBindTexture(GL_TEXTURE_2D, this->depthmap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, this->actualRes, this->actualRes, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		glBindFramebuffer(GL_FRAMEBUFFER, this->depthFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthmap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
public:
	DirLightShadow(){}
	DirLightShadow(glm::vec3 position)
	{
		this->position = position;
		this->actualRes;
		this->shadowProj = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, this->near_plane, this->far_plane);
		this->shadowView = glm::lookAt(this->position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	void render(Shader depthShader, void(*objectRenderingFunction)(Shader),
		void(*smokeRenderingFunction)(Shader, glm::vec3), Shader& smokeTodepthShader)
	{
		if (!enabled)
			return;
		if (this->actualRes != SHADOW_RES)
		{
			this->actualRes = SHADOW_RES;
			this->setupFBO();			
		}
		glViewport(0, 0, this->actualRes, this->actualRes);
		glBindFramebuffer(GL_FRAMEBUFFER, this->depthFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		depthShader.use();
		depthShader.setMat4(shadowProj * shadowView, "lightSpaceMatrix");
		objectRenderingFunction(depthShader);
		smokeTodepthShader.use();
		smokeTodepthShader.setMat4(shadowProj, "matProj");
		smokeTodepthShader.setMat4(shadowView, "matView");
		
		smokeRenderingFunction(smokeTodepthShader, this->position);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void attachDepthmap(Shader& shader, GLuint address = 30)
	{
		if (!enabled)
			return;
		glActiveTexture(GL_TEXTURE0 + address);
		glBindTexture(GL_TEXTURE_2D, this->depthmap);
		shader.setInt(address, "depthMapDir");
		shader.setMat4(shadowProj * shadowView, "lightSpaceMatrix");
	}
	void updateFarPlane(float val)
	{
		if (val == this->far_plane)
			return;
		this->far_plane = val;
		this->shadowProj = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, this->near_plane, this->far_plane);
		this->setupFBO();
		std::cout << "\n" << this->far_plane;
	}
};



class PointLightShadow 
{
private:
	GLuint depthFBO = 0;
	GLuint depthCubemap = 0;
	glm::vec3 position;
	glm::mat4 shadowProj;
	std::vector<glm::mat4> shadowTransforms;
	float actualRes;
	float near_plane = .3f;
	float far_plane = 15.0f;
	bool enabled;
	void setupFBO()
	{
		if (!this->depthFBO)
			glGenFramebuffers(1, &this->depthFBO);

		if (!this->depthCubemap)
			glGenTextures(1, &this->depthCubemap);

		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);

		for (int i = 0; i < 6; i++)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, this->actualRes, this->actualRes, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
		
		glBindFramebuffer(GL_FRAMEBUFFER, this->depthFBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->depthCubemap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void updateTransforms() 
	{
		this->shadowTransforms.push_back(shadowProj * glm::lookAt(this->position, this->position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		this->shadowTransforms.push_back(shadowProj * glm::lookAt(this->position, this->position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		this->shadowTransforms.push_back(shadowProj * glm::lookAt(this->position, this->position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		this->shadowTransforms.push_back(shadowProj * glm::lookAt(this->position, this->position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
		this->shadowTransforms.push_back(shadowProj * glm::lookAt(this->position, this->position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		this->shadowTransforms.push_back(shadowProj * glm::lookAt(this->position, this->position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	}

public:

	PointLightShadow(glm::vec3 position) 
	{
		this->position = position;
		this->position[1] += .2;
		this->actualRes;
		this->shadowProj = glm::perspective(glm::radians(90.0f), 1.f, near_plane, far_plane);
		this->updateTransforms();
	}

	void render(Shader depthShader, void(*objectRenderingFunction)(Shader))
	{
		if (!enabled)
			return;
		if (this->actualRes != SHADOW_RES) 
		{
			this->actualRes = SHADOW_RES;
			this->setupFBO();
		}

		glViewport(0, 0, this->actualRes, this->actualRes);
		glBindFramebuffer(GL_FRAMEBUFFER, this->depthFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		for (int i = 0; i < 6; i++)
			depthShader.setMat4(this->shadowTransforms[i], ("shadowMatrices[" + std::to_string(i) + "]").c_str());
		depthShader.setFloat(this->far_plane, "far_plane");
		depthShader.setVec3(this->position, "lightPos");
		objectRenderingFunction(depthShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void attachDepthmap(Shader& shader, GLuint index, GLuint address = 13)
	{
		if (!enabled)
			return;
		glActiveTexture(GL_TEXTURE0 + address);
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->depthCubemap);
		shader.setInt(address, ("depthMap[" + to_string(index) + "]").c_str());
		shader.setFloat(this->far_plane, ("far_plane[" + to_string(index) + "]").c_str());
	}
};