#pragma once


class LightManager 
{
private:
	unsigned pointLightMaxAmount;
	std::vector<PointLight> pointLights;
	DirLight dirLight;
	bool pointEnabled;
	float strenght;
	bool dirEnabled;

public:
	LightManager(unsigned maxPointLights) 
	{
		this->pointLightMaxAmount = maxPointLights;
		this->pointEnabled = true;
		this->strenght = 1.f;
	}
	LightManager(){}
	~LightManager(){}

	void attachPointLight(glm::vec3 pos, glm::vec3 color, float quad = 0.44, float linear = .35f, float constant = 1.0, bool isStatic = false, float str = 1.0) 
	{
		if (this->pointLights.size() < pointLightMaxAmount) {
			PointLight p;
			p.position = pos;
			p.color = color;
			p.quadratic = quad;
			p.linear = linear;
			p.constant = constant;
			p.str = str;
			p.isStatic = isStatic;
			this->pointLights.push_back(p);
		}
		else 
		{
			std::cout << "\n" << "Cannot add more point lights.";
		}
	}
	void setDirLight(glm::vec3 pos, glm::vec3 color) 
	{
		this->dirLight.position = pos;
		this->dirLight.color = color;
	}

	void enablePointLights(bool state) 
	{
		this->pointEnabled = state;
	}
	void enableDirLight(bool state)
	{
		this->dirEnabled = state;
	}
	void setPointStr(GLuint currFrame, GLuint maxFrame, float(*f)(GLuint, GLuint) = NULL) 
	{
		if (!f)
			return;

		this->strenght = f(currFrame, maxFrame);
	}
	void setUniforms(Shader &shader)
	{
		if (dirEnabled) {
			shader.setVec3(this->dirLight.position, "dirLight.position");
			shader.setVec3(this->dirLight.color, "dirLight.color");
		}
		else {
			shader.setVec3(glm::vec3(0, 0, 0), "dirLight.color");
		}


		for (GLuint i = 0; i < this->pointLights.size(); i++)
		{
			std::string number = std::to_string(i);

			if (!this->pointEnabled)
			{			
				shader.setFloat(0.f, ("pointLights[" + number + "].str").c_str());
				continue;
			}
			
			shader.setVec3(this->pointLights[i].color, ("pointLights[" + number + "].color").c_str());
			shader.setVec3(this->pointLights[i].position, ("pointLights[" + number + "].position").c_str());
			shader.setFloat(pointLights[i].quadratic, ("pointLights[" + number + "].quadratic").c_str());
			shader.setFloat(pointLights[i].linear, ("pointLights[" + number + "].linear").c_str());
			shader.setFloat(pointLights[i].constant, ("pointLights[" + number + "].constant").c_str());
			
			if (!this->pointLights[i].isStatic)
				shader.setFloat(this->pointLights[i].str * this->strenght, ("pointLights[" + number + "].str").c_str());
			else
				shader.setFloat(this->pointLights[i].str, ("pointLights[" + number + "].str").c_str());
		}
		

	}
	void quickSetup(std::vector<glm::vec3> positions) {

		//fire lights
	
		glm::vec3 color = glm::vec3(1.f, .5f, .5f);
		float quad = 0.017, linear = 0.07f, constant = 1.0;

		for (int i = 0; i < positions.size(); i++) 
		{
			glm::vec3 posMov = positions[i];
			posMov[1] += .2;
			this->attachPointLight(positions[i], color, quad, linear, constant, false, 1.0);
		}
		//light from top
		/*color = glm::vec3(0.8f, 0.9f, 0.9f);
		this->attachPointLight(glm::vec3(25, 30, -10), color, true, 0.0014, 0.000007, .15);*/
	}
};
float midMaxFunc(GLuint currFrame, GLuint maxFrame) {
	if (maxFrame == 0.f)
		return 1.f;
	return pow(cos(((float)currFrame - float(maxFrame) / 2) * 2 / maxFrame), 2);
}