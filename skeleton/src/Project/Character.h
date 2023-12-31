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

	const float ACTION_COLLISION_ZONE = 0.05;

	Camera* camera;
	bool canMoveCam;
	std::pair<int, int> cursor;
	Movement mov;
	bool isOnFloor;
	bool hasJumped;
	bool appliedMovForce;
	float elapsedTimeSlide;
	float elapsedTimeFall;
	PxTransform originalTransform;

	void adjustCamera() {
		// hacer que la camara se mueva con el raton
		canMoveCam = toggleClick();
		if (canMoveCam) {
			float shiftX, shiftY;
			shiftX = cursor.first - getCursor().first;
			shiftY = cursor.second - getCursor().second;
			cursor = getCursor();
			camera->shiftXAndY(shiftX * SHIFT_SMOOTHNESS, shiftY * SHIFT_SMOOTHNESS);
		}
		else {
			cursor = getCursor();
		}

		// hacer que la camara siga al personaje
		//camera->setEye(dynamicRigid->getGlobalPose().p + OFFSET_CAMERA);
	}

	void jump(double t) {
		// salto como tal
		if (mov.jump && isOnFloor) {
			dynamicRigid->addForce(Vector3(0, 1, 0) * getMasa() * JUMP_POWER, PxForceMode::eIMPULSE);
			isOnFloor = false;	// hack
			hasJumped = true;
		}
		// cuando llega al suelo es que ha dejado de saltar
		else if (hasJumped && isOnFloor) {
			hasJumped = false;
			elapsedTimeFall = 0;
		}

		// mientras esta saltando se aplica una pequeña fuerza para reducir el efecto de que flota
		if (hasJumped) {
			elapsedTimeFall += t;
			if (elapsedTimeFall >= FALL_TIMER) {
				dynamicRigid->addForce(Vector3(0, -1, 0) * getMasa() * FALL_POWER, PxForceMode::eIMPULSE);
				elapsedTimeFall = 0;
			}
		}
	}

	void move(double t) {
		if (mov.forward || mov.backward) {
			appliedMovForce = true;
			Vector3 forwardDir = camera->getDir();
			Vector3 straightForce = Vector3(forwardDir.x, 0, forwardDir.z) * getMasa() * MOV_POWER * t;
			if (mov.forward) {
				dynamicRigid->addForce(straightForce, physx::PxForceMode::eIMPULSE);
			}
			if (mov.backward) {
				dynamicRigid->addForce(-straightForce, physx::PxForceMode::eIMPULSE);
			}
		}

		if (mov.left || mov.right) {
			appliedMovForce = true;
			PxQuat rotation(physx::PxPi / 2, Vector3(0, 1, 0));
			Vector3 rightDir = rotation.rotate(camera->getDir());
			Vector3 sideForce = Vector3(rightDir.x, 0, rightDir.z) * getMasa() * MOV_POWER * t;
			if (mov.left) {
				dynamicRigid->addForce(sideForce, physx::PxForceMode::eIMPULSE);
			}
			if (mov.right) {
				dynamicRigid->addForce(-sideForce, physx::PxForceMode::eIMPULSE);
			}
		}

		// evitar el deslizamiento
		if (appliedMovForce) {
			elapsedTimeSlide += t;
			if (elapsedTimeSlide > AVOID_SLIDE_TIMER) {
				// evitar el deslizamiento cuando te has movido hasta caerte
				// (tb afecta al propio salto en carrera)
				dynamicRigid->addForce(Vector3(0, -1, 0) * getMasa() * ANTI_SLIDE_POWER, PxForceMode::eIMPULSE);
				elapsedTimeSlide = 0;

				// evitar deslizamiento cuando se anda
				if (isOnFloor && !mov.forward && !mov.backward && !mov.left && !mov.right) {
					Vector3 linearVel = dynamicRigid->getLinearVelocity();
					Vector3 angularVel = dynamicRigid->getAngularVelocity();
					dynamicRigid->setLinearVelocity(Vector3(0, linearVel.y, 0));
					dynamicRigid->setAngularVelocity(Vector3(0, angularVel.y, 0));

					appliedMovForce = false;
				}
			}
		}
	}

	virtual void onEnter() {
		if (detectOnFloor(colObject)) {
			isOnFloor = true;
		}
	}

	virtual void onExit() {
		isOnFloor = false;
	}

public:
	Character(PxPhysics* gPhysics, PxScene* gScene, const Vector3& pos, float density, float radius, float height) :
		CapsuleDynamicRB(gPhysics, gScene, pos, Vector3(0, 0, 0), Vector3(0, 0, 0), 0.8, density, Vector4(0, 1, 0, 1), radius, height,
			gPhysics->createMaterial(0.5, 0.8, 0), -1, true), camera(GetCamera()), canMoveCam(false), mov({ false, false, false, false, false }),
		isOnFloor(false), hasJumped(false), appliedMovForce(false), elapsedTimeSlide(0), elapsedTimeFall(0) {

		// girar la capsula para ponerla en vertical
		// (radianes, vector sobre el que rotarla)
		PxQuat rotation(physx::PxPi / 2, Vector3(0, 0, -1));
		originalTransform = PxTransform(dynamicRigid->getGlobalPose().p, rotation);
		dynamicRigid->setGlobalPose(originalTransform);

		// evitar que se vuelce en los ejes x y z
		dynamicRigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, true);
		dynamicRigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, true);

		// cursor inicial, por si la camara empieza con click activado
		cursor = getCursor();

		// impulso inicial hacia abajo para que caiga mas rapido
		dynamicRigid->addForce(Vector3(0, -1, 0) * getMasa() * INITIAL_FALL_POWER, PxForceMode::eIMPULSE);
	}

	void integrate(double t) {
		move(t);

		jump(t);

		// solo se salta una vez
		mov.jump = false;

		adjustCamera();

		// fuerza diminuta para que se sigan detectando las colisiones
		// (cuando pasa cierto tiempo las colisiones se paran)
		this->addForce(Vector3(0.00000001, 0, 0));
	}

	bool detectOnFloor(StaticRigidBody* floor) {
		return floor->getCeilingY() + ACTION_COLLISION_ZONE > getFloorY() &&
			getFloorY() > floor->getCeilingY() - ACTION_COLLISION_ZONE;
	}

	inline void resetChar() {
		this->dynamicRigid->setGlobalPose(originalTransform);
		isOnFloor = false;
		dynamicRigid->addForce(Vector3(0, -1, 0) * getMasa() * INITIAL_FALL_POWER, PxForceMode::eIMPULSE);
	}

	void keyPressed(char key) {
		if (isOnFloor) {
			if (key == 'I') {
				mov.forward = true;
			}
			if (key == 'K') {
				mov.backward = true;
			}
			if (key == 'J') {
				mov.left = true;
			}
			if (key == 'L') {
				mov.right = true;
			}
		}
		if (key == ' ') {
			mov.jump = true;
		}
	}

	void keyRelease(char key) {
		if (key == 'I') {
			mov.forward = false;
		}
		if (key == 'K') {
			mov.backward = false;
		}
		if (key == 'J') {
			mov.left = false;
		}
		if (key == 'L') {
			mov.right = false;
		}
	}
};