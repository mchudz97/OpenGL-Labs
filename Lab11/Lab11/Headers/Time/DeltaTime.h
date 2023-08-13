#pragma once
#include <chrono>
class DeltaTime 
{
private:
	std::chrono::time_point<std::chrono::system_clock> timestamp;
	double duration;
	bool updated = false;
public:
	DeltaTime(){}
	void update() 
	{
		std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
		duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - timestamp).count();
		timestamp = now;
		updated = true;
	}
	double getDelta() 
	{
		return this->duration;
	}

	void init() 
	{
		this->timestamp = std::chrono::system_clock::now();
	}
	int getFPS() 
	{
		return 1000 / this->duration;
	}
	string fpsInfo() 
	{
		if (!updated)
			this->update();
		this->updated = false;
		//string fps = string("FPS: ") + to_string(1000 / this->duration);
		return string("FPS: ") + to_string(int(1000 / this->duration));
	}

};