#pragma once
class ParticleGenerator 
{
private:
	GLfloat range;
	GLfloat maxHeight;
	GLfloat density;
	GLfloat velocity;
	InstantableMesh mesh;
	glm::vec3 position;
	GLuint layersToSpawn;
	GLfloat counter = 0.f;
	void addLayer() 
	{
		this->layersToSpawn--;
		std::vector<glm::mat4> layer;
		for (int i = 0; i < density; i++) 
		{
			float randR = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/range);
			float randP = rand();
			float randH = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / this->maxHeight);
			glm::mat4 origin = glm::translate(glm::mat4(1.f), this->position);
			layer.push_back(glm::translate(origin, glm::vec3(sin(randP) * randR, randH, cos(randP) * randR)));
		}
		mesh.attachMatrices(layer);
	}
	void refreshElements() 
	{
		std::vector<glm::mat4> actualPositions = this->mesh.getMatrices();
		for (int i = 0; i < actualPositions.size(); i++) 
		{
			GLfloat actY = actualPositions[i][3][1];
			if (actY > this->maxHeight + this->position[1])
				actualPositions[i][3][1] = this->position[1];
		}
		this->mesh.setMatrices(actualPositions);
		this->mesh.translate(glm::vec3(.0f, this->velocity, .0f));
	}
	void setUniforms(Shader &s) 
	{
		s.setFloat(this->maxHeight, "maxH");
		s.setFloat(this->counter, "actualStep");
		s.setVec3(this->position, "origin");
	}

public:

	ParticleGenerator(InstantableMesh particle, glm::vec3 sourcePos, GLfloat range, GLfloat maxHeight, GLuint layers, GLfloat density, GLfloat velocity, GLfloat hMove)
	{
		std::srand(std::time(0));
		this->mesh = particle;
		this->position = glm::vec3(sourcePos[0], sourcePos[1] + hMove, sourcePos[2]);
		this->range = range;
		this->maxHeight = maxHeight;
		this->layersToSpawn = layers;
		this->density = 1000 * density;
		this->velocity = velocity;
	}

	void render(Shader &shader, glm::vec3 camPos) {
		if (this->layersToSpawn > 0)
			this->addLayer();
		if (this->counter > this->maxHeight)
			this->counter = .0f;

		this->mesh.sortMatrices(camPos);
		this->setUniforms(shader);
		this->mesh.render(shader);
		//this->refreshElements();
		this->counter += this->velocity;
	}
	glm::vec3 getPosition() 
	{
		return this->position;
	}
};