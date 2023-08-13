#pragma once
#define OBJ_AMOUNT 8
#define UNREACH_AMOUNT 1
#define COLLECTABLE_AMOUNT 1
#define EMISSION_OBJ_AMOUNT 1
#define MULTIPLE_OBJ_AMOUNT 2
#define ROTATING_OBJ 1


const char* hdrPath = "Textures/Hdr/sky8.hdr";
//const char* hdrPath = "Textures/Hdr/sky3.hdr";

std::vector<string> names = {
	"kulke 1",
	"kulke 2"
};

std::vector<glm::vec3> firePositions = {
	glm::vec3(9.4021, 4.42164, -6.22175),
	glm::vec3(9.4021, 4.42164, 6.22175),
	glm::vec3(-9.4021, 4.42164, -6.22175),
	glm::vec3(-9.4021, 4.42164, 6.22175),
	glm::vec3(6.33911, 4.42164, -9.36901),
	glm::vec3(6.33911, 4.42164, 9.36901),
	glm::vec3(-6.33911, 4.42164, 9.36901),
	glm::vec3(-6.33911, 4.42164, -9.36901),
	glm::vec3(3.60202, 6.97752, -2.14058),
	glm::vec3(3.60202, 6.97752, 2.14058),
	glm::vec3(-3.60202, 6.97752, -2.14058),
	glm::vec3(-3.60202, 6.97752, 2.14058),
	glm::vec3(5.90828, 0.551271, -4.3801),
	glm::vec3(5.90828, 0.551271, 4.3801),
	glm::vec3(9.77278, 4.42164, -1.84826),
	glm::vec3(9.77278, 4.42164, 1.84826)

};

std::vector<glm::vec3> treasurePositions = {
	glm::vec3(11, 100, -5),
	glm::vec3(20, 100, 7),
	glm::vec3(1, 100, 22),
	glm::vec3(-8, 100, 17),
	glm::vec3(16, 100, -22),
	glm::vec3(24, 100, -11),
	glm::vec3(-10, 100, -10),
	glm::vec3(13, 100, 13),
	glm::vec3(15, 100, 20)
};

std::vector<glm::vec3> createLineOfPositions(glm::vec3 origin, float length, float distance, glm::vec3 direction)
{
	vector<glm::vec3> positions;
	glm::vec3 actualPos = origin;
	positions.push_back(actualPos);
	int amount = length / distance;
	for (int i = 0; i < amount; i++) {
		actualPos += direction * distance;
		positions.push_back(actualPos);
	}
	return positions;
}

const char* collectable[COLLECTABLE_AMOUNT] = {
	"Objects/treasure_box.obj"
};

const char* collectableTexNames[COLLECTABLE_AMOUNT * 5] = {
	"Textures/Wood/albedo.jpg",
	"Textures/Wood/roughness.jpg",
	"Textures/Wood/ao.jpg",
	"Textures/Wood/metal.png",
	"Textures/Wood/normal.jpg"
};

const char* unreachable[UNREACH_AMOUNT] = {
	"Objects/walls2.obj"
};

const char* unreachableTexNames[UNREACH_AMOUNT * 5] = {
	
	"Textures/Wall2/albedo.jpg",
	"Textures/Wall2/roughness.jpg",
	"Textures/Wall2/ao.jpg",
	"Textures/Wall2/metal.png",
	//"Textures/Wall2/normal.jpg",
	"Textures/Wall2/normal2.png"
};

const char* objNames[OBJ_AMOUNT] = {
	"Objects/castle.obj",
	"Objects/castle_floor.obj",
	"Objects/castle_roof.obj",
	"Objects/ground.obj",
	"Objects/doors.obj",
	"Objects/road.obj",
	"Objects/carriage.obj",
	"Objects/hay.obj"
};
const char* texNames[OBJ_AMOUNT * 5] = {
	"Textures/Wall2/albedo.jpg",
	"Textures/Wall2/roughness.jpg",
	"Textures/Wall2/ao.jpg",
	"Textures/Wall2/metal.png",
	"Textures/Wall2/normal.jpg",
	"Textures/Wood/albedo.jpg",
	"Textures/Wood/roughness.jpg",
	"Textures/Wood/ao.jpg",
	"Textures/Wood/metal.png",
	"Textures/Wood/normal.jpg",
	"Textures/Roof/albedo.jpg",
	"Textures/Roof/roughness.jpg",
	"Textures/Roof/ao.jpg",
	"Textures/Roof/metal.png",
	"Textures/Roof/normal.jpg",
	"Textures/Ground_winter/albedo.jpg",
	"Textures/Ground_winter/roughness.png",
	"Textures/Ground_winter/ao.png",
	"Textures/Ground_winter/metal.png",
	"Textures/Ground_winter/normal.png",
	"Textures/Door/albedo.jpg",
	"Textures/Door/roughness.jpg",
	"Textures/Door/ao.jpg",
	"Textures/Door/metal.png",
	"Textures/Door/normal.jpg",
	"Textures/Wall2/albedo.jpg",
	"Textures/Wall2/roughness.jpg",
	"Textures/Wall2/ao.jpg",
	"Textures/Wall2/metal.png",
	//"Textures/Wall2/normal.jpg",
	"Textures/Wall2/normal2.png",
	"Textures/Wood_procedural/albedo.png",
	"Textures/Wood_procedural/roughness.png",
	"Textures/Wood_procedural/ao.png",
	"Textures/Wood_procedural/metal.png",
	"Textures/Wood_procedural/normal.png",
	"Textures/Hay/albedo.jpg",
	"Textures/Hay/roughness.jpg",
	"Textures/Hay/ao.jpg",
	"Textures/Hay/metal.png",
	"Textures/Hay/normal.jpg",
};
const char* multipleObj[MULTIPLE_OBJ_AMOUNT] = {
	"Objects/vessel.obj",
	"Objects/sticks.obj"
};
const char* multipleObjTextures[MULTIPLE_OBJ_AMOUNT * 5] = {
	"Textures/Vessel/albedo.png",
	"Textures/Vessel/roughness.png",
	"Textures/Vessel/ao.png",
	"Textures/Vessel/metal.png",
	"Textures/Vessel/normal.png",
	"Textures/Sticks/albedo.jpg",
	"Textures/Sticks/roughness.png",
	"Textures/Sticks/ao.jpg",
	"Textures/Sticks/metal.png",
	"Textures/Sticks/normal.jpg"
};
const char* emissionObjNames[EMISSION_OBJ_AMOUNT] = {
	"Objects/sprite2.obj"
};
const char* emissionTexNames[EMISSION_OBJ_AMOUNT] = {
	"Textures/sprite.png"
};

const char* rotatingObjNames[ROTATING_OBJ] = {
	"Objects/sphere.obj",
};
const char* rotatingTexNames[ROTATING_OBJ * 5] = {
	"Textures/Sphere/albedo.png",
	"Textures/Sphere/roughness.png",
	"Textures/Sphere/ao.png",
	"Textures/Sphere/metal.png",
	"Textures/Sphere/normal2.png",
};

