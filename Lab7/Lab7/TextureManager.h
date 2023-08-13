#pragma once

class TextureManager 
{
private:
	std::vector<Texture*> textures;
public:

	TextureManager(){}

	Texture* getTexture(const char* path, bool Anisotropy, unsigned frames = 0)
	{
		if (!path)
			return NULL;
		for (int i = 0; i < textures.size(); i++) 
		{
			if (strcmp(textures[i]->getPath(), path) == 0)
				return textures[i];
		}

		Texture* t = new Texture();
		t->setup(path, Anisotropy, frames);
		this->textures.push_back(t);
		return t;
	}

};