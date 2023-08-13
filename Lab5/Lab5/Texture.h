#pragma once
class Texture 
{
private:
	GLuint idTex;
	int frames;
	int tex_width, tex_height, n;
	unsigned char* tex_data;
	const char* path;
	int actualFrame;
	bool removeBlack;
	bool load() 
	{
		stbi_set_flip_vertically_on_load(true); 

		tex_data = stbi_load(path, &tex_width, &tex_height, &n, 0);
		if (tex_data == NULL)
		{
			printf("Image can't be loaded!\n");
			exit(1);
		}

		std::string format = std::string(path);
		int dotPos = format.find(".");
		while (dotPos != -1) {
			format = format.substr(dotPos + 1, format.size() - 1);
			dotPos = format.find(".");
		}
		
		glGenTextures(1, &this->idTex);
		glBindTexture(GL_TEXTURE_2D, this->idTex);
		if (format != "jpeg" || format != "png")
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
		stbi_image_free(tex_data);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
public:


	Texture(const char* path, int frames = 0, bool removeBlack = false)
	{
		if (path)
			this->path = path;

		this->removeBlack = removeBlack;
		this->frames = frames;
		this->load();

	}
	~Texture(){}
	void display(GLuint idProgram) {
		if (this->removeBlack) 
			glUniform1i(glGetUniformLocation(idProgram, "alphaTex"), 1);
		else
			glUniform1i(glGetUniformLocation(idProgram, "alphaTex"), 0);
		if (this->frames) 
		{
			glUniform1f(glGetUniformLocation(idProgram, "texMov"), float(this->actualFrame)/this->frames);
			this->actualFrame = (this->actualFrame + 1) % this->frames;
		}
		else 
		{
			glUniform1f(glGetUniformLocation(idProgram, "texMov"), 0.0f);
		}
		glUniform1i(glGetUniformLocation(idProgram, "tex"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->idTex);
	}
	bool RemovesBlack() {
		return this->removeBlack;
	}
};