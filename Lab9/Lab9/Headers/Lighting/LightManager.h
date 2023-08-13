#pragma once


class LightManager 
{
private:
	unsigned pointLightMaxAmount;
	std::vector<PointLight> pointLights;
	bool pointEnabled;
	float strenght;

public:
	LightManager(unsigned maxPointLights) 
	{
		this->pointLightMaxAmount = maxPointLights;
		this->pointEnabled = true;
		this->strenght = 1.f;
	}
	LightManager(){}
	~LightManager(){}

	void attachPointLight(glm::vec3 pos, glm::vec3 color, bool isStatic = false, float str = 1.0) 
	{
		if (this->pointLights.size() < pointLightMaxAmount) {
			PointLight p;
			p.position = pos;
			p.color = color;
			p.str = str;
			p.isStatic = isStatic;
			this->pointLights.push_back(p);
		}
		else 
		{
			std::cout << "\n" << "Cannot add more point lights.";
		}
	}

	void enablePointLights(bool state) {
		this->pointEnabled = state;
	}

	void setPointStr(GLuint currFrame, GLuint maxFrame, float(*f)(GLuint, GLuint) = NULL) 
	{
		if (!f)
			return;

		this->strenght = f(currFrame, maxFrame);
	}
	void setUniforms(GLuint idProgram)
	{
		
		for (GLuint i = 0; i < this->pointLights.size(); i++)
		{
			std::string number = std::to_string(i);

			glUniform3fv(glGetUniformLocation(idProgram, ("pointLights[" + number + "].position").c_str()), 1, glm::value_ptr(this->pointLights[i].position));
			
			if (this->pointEnabled)
			{
				glUniform3fv(glGetUniformLocation(idProgram,("pointLights[" + number + "].color").c_str()),1,
							glm::value_ptr(this->pointLights[i].color));
			}
			else 
			{
				glUniform3fv(glGetUniformLocation(idProgram, ("pointLights[" + number + "].color").c_str()), 1, glm::value_ptr(glm::vec3(0.f)));
			}
			if (!this->pointLights[i].isStatic)
				glUniform1f(glGetUniformLocation(idProgram, ("pointLights[" + number + "].str").c_str()), this->pointLights[i].str * this->strenght);
			else
				glUniform1f(glGetUniformLocation(idProgram, ("pointLights[" + number + "].str").c_str()), this->pointLights[i].str);
		}
		

	}
	void quickSetup(std::vector<glm::vec3> positions) {

		//fire lights
	
		glm::vec3 color = glm::vec3(1.f, .5f, .5f);

		for (int i = 0; i < positions.size(); i++) 
		{
			this->attachPointLight(positions[i], color, false, 4.0);
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