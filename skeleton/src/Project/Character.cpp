#include "Character.h"
#include "Scene.h"

void Character::adjustCamera() {
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
	camera->setEye(dynamicRigid->getGlobalPose().p + OFFSET_CAMERA);
}

void Character::airbone(double t) {
	// salto
	if (mov.jump && isOnFloor) {
		dynamicRigid->addForce(Vector3(0, 1, 0) * getMasa() * JUMP_POWER, PxForceMode::eIMPULSE);
		//isOnFloor = false;	hack
		//hasJumped = true;
	}

	// mientras esta en el aire se aplica una pequeña fuerza constante para evitar el efecto de que flota
	if (!isOnFloor) {	// antes hasJumped
		elapsedTimeFall += t;
		if (elapsedTimeFall >= FALL_TIMER) {
			dynamicRigid->addForce(Vector3(0, -1, 0) * getMasa() * FALL_POWER, PxForceMode::eIMPULSE);
			elapsedTimeFall = 0;
		}
	}
	// cuando llega al suelo se reinicia el contador de esta fuerza
	else if (isOnFloor) {	// antes && hasJumped
		elapsedTimeFall = 0;
	}
}

void Character::move(double t) {
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
		Vector3 leftDir = camera->getLeftDir();
		Vector3 sideForce = Vector3(leftDir.x, 0, leftDir.z) * getMasa() * MOV_POWER * t;
		if (mov.left) {
			dynamicRigid->addForce(sideForce, physx::PxForceMode::eIMPULSE);
		}
		if (mov.right) {
			dynamicRigid->addForce(-sideForce, physx::PxForceMode::eIMPULSE);
		}
	}

	// EVITAR EL DESLIZAMIENTO
	if (appliedMovForce) {
		elapsedTimeSlide += t;
		if (elapsedTimeSlide > AVOID_SLIDE_TIMER) {
			// evitar el deslizamiento cuando te has movido hasta caerte
			// (tb afecta al propio salto en carrera)
			dynamicRigid->addForce(Vector3(0, -1, 0) * getMasa() * ANTI_SLIDE_POWER, PxForceMode::eIMPULSE);
			elapsedTimeSlide = 0;

			// evitar deslizamiento cuando se anda (efecto stop)
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

void Character::onEnter() {
	if (detectOnFloor(colObject)) {
		isOnFloor = true;
	}
}

void Character::onExit() {
	isOnFloor = false;
}

Character::Character(PxPhysics* gPhysics, PxScene* gScene, const Vector3& pos, float density, float radius, float height) :
	CapsuleDynamicRB(gPhysics, gScene, pos, Vector3(0, 0, 0), Vector3(0, 0, 0), 0.8, density, Vector4(0, 1, 0, 1), radius, height,
		gPhysics->createMaterial(0.5, 0.8, 0), -1, Scene::get()->DEBUG), camera(GetCamera()), canMoveCam(false), mov({ false, false, false, false, false }),
	isOnFloor(false), appliedMovForce(false), elapsedTimeSlide(0), elapsedTimeFall(0), haveInput(true) {

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

void Character::integrate(double t) {
	move(t);

	airbone(t);

	// solo se salta una vez
	mov.jump = false;

	adjustCamera();

	// fuerza diminuta para que se sigan detectando las colisiones
	// (cuando pasa cierto tiempo las colisiones se paran)
	this->addForce(Vector3(0.00000001, 0, 0));
}

bool Character::detectOnFloor(StaticRigidBody* floor) {
	float floorCeilingY = floor->getCeilingY();
	float playerfloorY = getFloorY();
	return floorCeilingY + ACTION_COLLISION_ZONE > playerfloorY &&
		playerfloorY > floorCeilingY - ACTION_COLLISION_ZONE;
}

void Character::keyPressed(char key) {
	if (haveInput) {
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
	else {
		mov.forward = mov.backward = mov.left = mov.right = mov.jump = false;
	}
}

void Character::keyRelease(char key) {
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