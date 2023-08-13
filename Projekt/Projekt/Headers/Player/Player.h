#pragma once
enum BODY_PARTS 
{
	HEAD,
	NECK,
	CHEST,
	ARM_LEFT,
	ARM_RIGHT,
	FOREARM_LEFT,
	FOREARM_RIGHT,
	HAND_LEFT,
	HAND_RIGHT,
	THIGH_LEFT,
	THIGH_RIGHT,
	SHANK_LEFT,
	SHANK_RIGHT,
	FEET_LEFT,
	FEET_RIGHT
};

enum POSE_STATES 
{
	STANDING,
	MOVING
};

class Player 
{
private:
	glm::mat4 transform = glm::mat4(1.0f);
	GLfloat rot = 0.f;
	glm::vec3 pos;
	Camera playerCam;
public:
	
	Player(GLfloat cameraDist) 
	{
		this->playerCam = Camera(cameraDist);
	}

	std::map<BODY_PARTS, BodyPart*> bodyElements;

	void addBodyPart(BODY_PARTS partType, BodyPart* part) 
	{
		this->bodyElements[partType] = part;
	}

	void render(Shader& shader) 
	{
		this->bodyElements[ARM_LEFT]->rotate(glm::vec3(1, 0, 0), 0.01);
		this->bodyElements[CHEST]->render(shader, this->transform);
	}
	
	void shadowRender(Shader& shader) 
	{
		this->bodyElements[CHEST]->shadowRender(shader, this->transform);
	}

	void move(GLfloat dir, CGround& cg, std::vector<CollidingMesh> ureachablePlacs)
	{
		glm::vec3 mov = glm::vec3(dir * sin(this->rot), 0, dir * cos(this->rot));
		for (CollidingMesh cm : unreachablePlaces) 
		{
			if (cm.isColliding(pos + mov))
				return;
		}
		cg.checkMove(pos, pos + mov);

		//this->pos += mov;
		this->transform = glm::translate(glm::mat4(1.f), this->pos);

	}
	void rotate(GLfloat angle) 
	{
		this->rot += angle;
		this->transform = glm::rotate(this->transform, rot, glm::vec3(0, 1, 0));
	}
	glm::mat4 getView() 
	{
		this->playerCam.calculateView(this->pos, this->rot);
		return this->playerCam.getView();
	}
	glm::vec3 getPos() 
	{
		return this->pos;
	}
	void setPos(glm::vec3 pos) 
	{
		this->pos = pos;
	}
};

Player* createHumanPlayer(TextureManager& tm) {
	Player* p = new Player(2.f);
	BodyPart* chest = new BodyPart(createTexturedMesh("Objects/Player/chest.obj", tm, true, "Textures/Player/kratka.png"));
	BodyPart* neck = new BodyPart(createTexturedMesh("Objects/Player/neck.obj", tm, true, "Textures/Player/skin_face_body.png"), chest);
	BodyPart* head = new BodyPart(createTexturedMesh("Objects/Player/head.obj", tm, true, "Textures/Player/skin_face_body.png"), neck);
	BodyPart* arm_l = new BodyPart(createTexturedMesh("Objects/Player/arm_l.obj", tm, true, "Textures/Player/kratka.png"), chest);
	BodyPart* arm_r = new BodyPart(createTexturedMesh("Objects/Player/arm_r.obj", tm, true, "Textures/Player/kratka.png"), chest);
	BodyPart* forearm_l = new BodyPart(createTexturedMesh("Objects/Player/forearm_l.obj", tm, true, "Textures/Player/kratka.png"), arm_l);
	BodyPart* forearm_r = new BodyPart(createTexturedMesh("Objects/Player/forearm_r.obj", tm, true, "Textures/Player/kratka.png"), arm_r);
	BodyPart* hand_l = new BodyPart(createTexturedMesh("Objects/Player/hand_l.obj", tm, true, "Textures/Player/skin_face_body.png"), forearm_l);
	BodyPart* hand_r = new BodyPart(createTexturedMesh("Objects/Player/hand_r.obj", tm, true, "Textures/Player/skin_face_body.png"), forearm_r);
	BodyPart* thigh_l = new BodyPart(createTexturedMesh("Objects/Player/thigh_l.obj", tm, true, "Textures/Player/koleczka.png"), chest);
	BodyPart* thigh_r = new BodyPart(createTexturedMesh("Objects/Player/thigh_r.obj", tm, true, "Textures/Player/koleczka.png"), chest);
	BodyPart* shank_l = new BodyPart(createTexturedMesh("Objects/Player/shank_l.obj", tm, true, "Textures/Player/koleczka.png"), thigh_l);
	BodyPart* shank_r = new BodyPart(createTexturedMesh("Objects/Player/shank_r.obj", tm, true, "Textures/Player/koleczka.png"), thigh_r);
	BodyPart* feet_l = new BodyPart(createTexturedMesh("Objects/Player/feet_l.obj", tm, true, "Textures/Player/but.png"), shank_l);
	BodyPart* feet_r = new BodyPart(createTexturedMesh("Objects/Player/feet_r.obj", tm, true, "Textures/Player/but.png"), shank_r);


	p->addBodyPart(CHEST, chest);
	p->addBodyPart(NECK, neck);
	p->addBodyPart(HEAD, head);
	p->addBodyPart(ARM_LEFT, arm_l);
	p->addBodyPart(ARM_RIGHT, arm_r);
	p->addBodyPart(FOREARM_LEFT, forearm_l);
	p->addBodyPart(FOREARM_RIGHT, forearm_r);
	p->addBodyPart(HAND_LEFT, hand_l);
	p->addBodyPart(HAND_RIGHT, hand_r);
	p->addBodyPart(THIGH_LEFT, thigh_l);
	p->addBodyPart(THIGH_RIGHT, thigh_r);
	p->addBodyPart(SHANK_LEFT, shank_l);
	p->addBodyPart(SHANK_RIGHT, shank_r);
	p->addBodyPart(FEET_LEFT, feet_l);
	p->addBodyPart(FEET_RIGHT, feet_r);

	p->bodyElements[CHEST]->translate(glm::vec3(.0f, 1.66f, -.004f));
	p->bodyElements[NECK]->translate(glm::vec3(.0f, .32f, -.032f));
	p->bodyElements[HEAD]->translate(glm::vec3(.0f, .13f, .0f));
	p->bodyElements[ARM_LEFT]->translate(glm::vec3(.225f, .26f, .001f));
	p->bodyElements[ARM_RIGHT]->translate(glm::vec3(-.225f, .26f, .001f));
	p->bodyElements[FOREARM_LEFT]->translate(glm::vec3(.25f, -.34f, -.026f));
	p->bodyElements[FOREARM_RIGHT]->translate(glm::vec3(-.25f, -.34f, -.026f));
	p->bodyElements[HAND_LEFT]->translate(glm::vec3(.1f, -.41f, -.01f));
	p->bodyElements[HAND_RIGHT]->translate(glm::vec3(-.1f, -.41f, -.01f));
	p->bodyElements[THIGH_LEFT]->translate(glm::vec3(.19f, -.56f, .01f));
	p->bodyElements[THIGH_RIGHT]->translate(glm::vec3(-.19f, -.56f, .01f));
	p->bodyElements[SHANK_LEFT]->translate(glm::vec3(.03f, -.45f, .0f));
	p->bodyElements[SHANK_RIGHT]->translate(glm::vec3(-.03f, -.45f, .0f));
	p->bodyElements[FEET_LEFT]->translate(glm::vec3(.01f, -.53f, .0f));
	p->bodyElements[FEET_RIGHT]->translate(glm::vec3(-.01f, -.53f, .0f));
	p->setPos(glm::vec3(15, 2, 0));
	return p;
}