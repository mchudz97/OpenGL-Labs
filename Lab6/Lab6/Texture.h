#pragma once
enum textureType {
	DEFAULT,
	REMOVE_BLACK,
	BUMP,
	DIFFUSE,
	SPECULAR
};

class Texture
{
private:
	GLuint idTex;
	int frames;
	
	const char* path;
	int actualFrame;

	int adress;
	int channel;
	const char* uniformName;
	
	textureType type;
	
	bool load()
	{
		int tex_width, tex_height, n;
		unsigned char* tex_data;
		stbi_set_flip_vertically_on_load(true);

		tex_data = stbi_load(path, &tex_width, &tex_height, &n, 0);
		if (tex_data == NULL)
		{
			printf("Image can't be loaded!\n");
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
		glGenerateMipmap(GL_TEXTURE_2D);
		glActiveTexture(0);
	}
public:

	void setup(const char* path, int frames = 0, textureType texType = textureType::DEFAULT) 
	{
		if (path)
			this->path = path;
		this->frames = frames;
		this->type = texType;
		if (texType == textureType::DEFAULT || texType == textureType::REMOVE_BLACK) 
		{
			this->channel = 0;
			this->adress = GL_TEXTURE0;
			this->uniformName = "tex";
		}
		else if (texType == textureType::DIFFUSE) 
		{
			this->channel = 1;
			this->adress = GL_TEXTURE1;
			this->uniformName = "material.diffuse";
		}
		else if (texType == textureType::SPECULAR)
		{
			this->channel = 2;
			this->adress = GL_TEXTURE2;
			this->uniformName = "material.specular";
		}
		else if (texType == textureType::BUMP)
		{
			this->channel = 3;
			this->adress = GL_TEXTURE3;
			this->uniformName = "material.normal";
		}
		this->load();
	}

	Texture(const char* path, int frames = 0, textureType texType = textureType::DEFAULT)
	{
		this->setup(path, frames, texType);
		
	}
	Texture(){}
	~Texture(){}

	
	void activate(GLuint idProgram) {
		
		if (!this->adress)
			return;
		if (this->type == textureType::REMOVE_BLACK) 
		{
			glEnable(GL_BLEND);
			glUniform1i(glGetUniformLocation(idProgram, "alphaTex"), 1);
		}
			
		else 
		{
			glDisable(GL_BLEND);
			glUniform1i(glGetUniformLocation(idProgram, "alphaTex"), 0);		
		}

		glActiveTexture(this->adress);
		glBindTexture(GL_TEXTURE_2D, this->idTex);
		glUniform1i(glGetUniformLocation(idProgram, this->uniformName), this->channel);
			
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
	void deactivate() {
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};