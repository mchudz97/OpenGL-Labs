#pragma once

enum textureType {
	DEFAULT,
	BUMP,
	ALBEDO,
	ROUGHNESS,
	AO,
	METAL
};

GLfloat maxAnisotropy = -1.0f;

class Texture
{
private:
	GLuint idTex;
	int frames;

	const char* path;
	int actualFrame;



	bool load( bool Anisotropy)
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

		glGenTextures(1, &this->idTex);
		glBindTexture(GL_TEXTURE_2D, this->idTex);

		glTexImage2D(GL_TEXTURE_2D, 0, format, tex_width, tex_height, 0, format, GL_UNSIGNED_BYTE, tex_data);
		stbi_image_free(tex_data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		if (maxAnisotropy > 0.0f)
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, (Anisotropy == true) ? maxAnisotropy : 1.0f);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
		glActiveTexture(0);
	}
public:

	void setup(const char* path, bool Anisotropy, int frames = 0)
	{
		if (path)
			this->path = path;
		this->frames = frames;
		this->load(Anisotropy);
	}

	//Texture(const char* path, int frames = 0)
	//{
	//	this->setup(path, frames);
	//}
	Texture() {}
	~Texture() {}


	void activate(GLuint idProgram, textureType texType = textureType::DEFAULT) {

		GLuint channel;
		GLuint adress;
		const char* uniformName;

		if (texType == textureType::DEFAULT)
		{
			channel = 0;
			adress = GL_TEXTURE0;
			uniformName = "tex";
		}
		else if (texType == textureType::ALBEDO)
		{
			channel = 1;
			adress = GL_TEXTURE1;
			uniformName = "material.albedo";
		}
		else if (texType == textureType::ROUGHNESS)
		{
			channel = 2;
			adress = GL_TEXTURE2;
			uniformName = "material.roughness";
		}
		else if (texType == textureType::AO)
		{
			channel = 3;
			adress = GL_TEXTURE3;
			uniformName = "material.ao";
		}
		else if (texType == textureType::METAL)
		{
			channel = 4;
			adress = GL_TEXTURE4;
			uniformName = "material.metallic";
		}
		else
		{
			channel = 5;
			adress = GL_TEXTURE5;
			uniformName = "material.normal";
		}


		glActiveTexture(adress);
		glBindTexture(GL_TEXTURE_2D, idTex);
		glUniform1i(glGetUniformLocation(idProgram, uniformName), channel);

		if (this->frames)
		{
			glUniform1f(glGetUniformLocation(idProgram, "texMov"), float(this->actualFrame) / this->frames);
			this->actualFrame = (this->actualFrame + 1) % this->frames;
		}
		else
		{
			glUniform1f(glGetUniformLocation(idProgram, "texMov"), 0.0f);
		}

	}
	void deactivate() 
	{
		glActiveTexture(0);
	}
	const char* getPath() 
	{
		return this->path;
	}
};