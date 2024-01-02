#pragma once
#include "./CapsuleDynamicRB.h"

class Character : public CapsuleDynamicRB {
private:
	struct Movement {
		bool forward;
		bool backward;
		bool left;
		bool right;
		bool jump;
	};

	// la camara principal es la q sigue al jugador
	const float SHIFT_SMOOTHNESS = 0.5;
	const Vector3 OFFSET_CAMERA = Vector3(0, 10, 0);

	const float MOV_POWER = 150;
	const float INITIAL_FALL_POWER = 70;
	const float ANTI_SLIDE_POWER = 1;
	const float JUMP_POWER = 80;
	const float FALL_POWER = 5;

	const float AVOID_SLIDE_TIMER = 0.06f;
	const float FALL_TIMER = 0.04f;

	const float ACTION_COLLISION_ZONE = 0.5;

	Camera* camera;
	bool canMoveCam;
	std::pair<int, int> cursor;
	Movement mov;
	bool isOnFloor;
	bool appliedMovForce;
	bool haveInput;
	float elapsedTimeSlide;
	float elapsedTimeFall;
	PxTransform originalTransform;

	void adjustCamera();

	void airbone(double t);

	void move(double t);

	virtual void onEnter();

	virtual void onExit();

public:
	Character(PxPhysics* gPhysics, PxScene* gScene, const Vector3& pos, float density, float radius, float height);

	void integrate(double t);

	bool detectOnFloor(StaticRigidBody* floor);

	inline void resetChar() {
		this->dynamicRigid->setGlobalPose(originalTransform);
		isOnFloor = false;
		dynamicRigid->addForce(Vector3(0, -1, 0) * getMasa() * INITIAL_FALL_POWER, PxForceMode::eIMPULSE);
		haveInput = true;
	}

	void Character::setInput(bool enable) {
		this->haveInput = enable;
	}

	void keyPressed(char key);

	void keyRelease(char key);
};