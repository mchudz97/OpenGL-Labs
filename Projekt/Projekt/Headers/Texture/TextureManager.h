#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
GLfloat maxAnisotropy = -1.0f;
class TextureManager 
{
private:
	std::map<std::string, GLuint> textures;
	GLuint hdrTexId, envCubemap, irradianceMap, prefilterMap, brdfTexture;
	GLuint capFBO, capRBO;
	glm::mat4 capViews[6] = 
	{
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};
	glm::mat4 capProjection;

	Mesh* hdrCube;

	bool load(GLuint &id, const char* path, bool Anisotropy)
	{
		int tex_width, tex_height, n;
		unsigned char* tex_data;
		stbi_set_flip_vertically_on_load(true);

		tex_data = stbi_load(path, &tex_width, &tex_height, &n, 0);
		if (tex_data == NULL)
		{
			printf("Image %s can't be loaded!\n", path);
			exit(1);
		}
		GLenum format;
		if (n == 1)
			format = GL_RED;
		else if (n == 3)
			format = GL_RGB;
		else if (n == 4)
			format = GL_RGBA;

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexImage2D(GL_TEXTURE_2D, 0, format, tex_width, tex_height, 0, format, GL_UNSIGNED_BYTE, tex_data);
		stbi_image_free(tex_data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		if (maxAnisotropy > 0.0f)
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, (Anisotropy == true) ? maxAnisotropy : 1.0f);
		}
		glGenerateMipmap(GL_TEXTURE_2D);

	}


public:

	TextureManager(){}

	GLuint getTexture(const char* path, bool Anisotropy)
	{
		if (!path)
			return NULL;
		if (this->textures.count(string(path)))
			return this->textures.find(string(path))->second;

		GLuint id;
		this->load(id, path, Anisotropy);
		this->textures[string(path)] = id;
		//std::cout << "\n " << path << " id : " << id;
		return id;
	}
	void setupHDR(const char* path)
	{

		stbi_set_flip_vertically_on_load(true);
		int width, height, n;
		float* data = stbi_loadf(path, &width, &height, &n, 0);
		if (!data)
		{
			std::cout << "\n HDR " << path << " could not be open.";
			return;
		}
		glGenTextures(1, &this->hdrTexId);
		glBindTexture(GL_TEXTURE_2D, this->hdrTexId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);

		glGenFramebuffers(1, &this->capFBO);
		glGenRenderbuffers(1, &this->capRBO);
		glBindFramebuffer(GL_FRAMEBUFFER, this->capFBO);
		glBindRenderbuffer(GL_RENDERBUFFER, this->capRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 1024, 1024);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->capRBO);

		glGenTextures(1, &this->envCubemap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->envCubemap);
		for (unsigned int i = 0; i < 6; ++i)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
				1024, 1024, 0, GL_RGB, GL_FLOAT, nullptr);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		this->capProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	}

	void bindHdrToHdrShader(Shader shader, Mesh *cube)
	{
		this->hdrCube = cube;
		shader.use();
		shader.setInt(0, "hdrMap");
		shader.setMat4(this->capProjection, "matProj");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->hdrTexId);

		glViewport(0, 0, 1024, 1024);
		glBindFramebuffer(GL_FRAMEBUFFER, this->capFBO);
		for (unsigned int i = 0; i < 6; ++i)
		{
			shader.setMat4(this->capViews[i], "matView");
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, this->envCubemap, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			this->hdrCube->render(shader);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->envCubemap);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}


	void setupIrradiance(Shader program_irradiance) 
	{
		glGenTextures(1, &this->irradianceMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->irradianceMap);
		for (int i = 0; i < 6; i++) 
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindFramebuffer(GL_FRAMEBUFFER, this->capFBO);
		glBindRenderbuffer(GL_RENDERBUFFER, this->capRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

		program_irradiance.use();
		program_irradiance.setInt(0, "envMap");
		program_irradiance.setMat4(this->capProjection, "matProj");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->envCubemap);
		glViewport(0, 0, 32, 32);
		glBindFramebuffer(GL_FRAMEBUFFER, this->capFBO);
		for (int i = 0; i < 6; ++i)
		{
			program_irradiance.setMat4(capViews[i], "matView");
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, this->irradianceMap, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			this->hdrCube->render(program_irradiance);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}


	void setupPrefilter(Shader prefilterShader)
	{
		glGenTextures(1, &this->prefilterMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->prefilterMap);
		for (unsigned int i = 0; i < 6; ++i)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		prefilterShader.use();
		prefilterShader.setInt(0, "envMap");
		prefilterShader.setMat4(this->capProjection, "matProj");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->envCubemap);
		glBindFramebuffer(GL_FRAMEBUFFER, this->capFBO);
		
		unsigned int maxMipLevels = 5;
		for (int m = 0; m < maxMipLevels; ++m) 
		{
			unsigned int mipWidth = 128 * pow(0.5, m);
			unsigned int mipHeight = 128 * pow(0.5, m);
			glBindRenderbuffer(GL_RENDERBUFFER, this->capRBO);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
			glViewport(0, 0, mipWidth, mipHeight);

			float roughness = (float)m / (float)(maxMipLevels - 1);
			prefilterShader.setFloat(roughness, "roughness");
			for (unsigned int i = 0; i < 6; ++i)
			{
				prefilterShader.setMat4(this->capViews[i], "matView");
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, this->prefilterMap, m);

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				this->hdrCube->render(prefilterShader);
			}
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void renderQuad()
	{
		unsigned int quadVAO = 0;
		unsigned int quadVBO;
		if (quadVAO == 0)
		{
			float quadVertices[] = {
				// positions        // texture Coords
				-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
				 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			};
			// setup plane VAO
			glGenVertexArrays(1, &quadVAO);
			glGenBuffers(1, &quadVBO);
			glBindVertexArray(quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		}
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}


	void setupBRDF(Shader brdfShader) 
	{
		glGenTextures(1, &this->brdfTexture);

		glBindTexture(GL_TEXTURE_2D, this->brdfTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
		// be sure to set wrapping mode to GL_CLAMP_TO_EDGE
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
		glBindFramebuffer(GL_FRAMEBUFFER, this->capFBO);
		glBindRenderbuffer(GL_RENDERBUFFER, this->capRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->brdfTexture, 0);

		glViewport(0, 0, 512, 512);
		brdfShader.use();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderQuad();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void setupCompleteHDR(const char* hdrImagePath, Shader hdr, Mesh* cube, Shader irradiance, Shader prefilter, Shader brdf) 
	{
		this->setupHDR(hdrImagePath);
		this->bindHdrToHdrShader(hdr, cube);
		this->setupIrradiance(irradiance);
		this->setupPrefilter(prefilter);
		this->setupBRDF(brdf);
	}

	void activateHDR(Shader shader)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->envCubemap);
		shader.setInt(0, "hdrMap");
		this->hdrCube->render(shader);
	}


	void activateHdrTextures(Shader pbrShader, GLuint begginingAdress = 10) 
	{
		pbrShader.use();
		glActiveTexture(GL_TEXTURE0 + begginingAdress);
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->irradianceMap);
		pbrShader.setInt(begginingAdress, "irradianceMap");

		glActiveTexture(GL_TEXTURE0 + begginingAdress + 1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->prefilterMap);
		pbrShader.setInt(begginingAdress + 1, "prefilterMap");

		glActiveTexture(GL_TEXTURE0 + begginingAdress + 2);
		glBindTexture(GL_TEXTURE_2D, this->brdfTexture);
		pbrShader.setInt(begginingAdress + 2, "brdfLUT");
	}
};