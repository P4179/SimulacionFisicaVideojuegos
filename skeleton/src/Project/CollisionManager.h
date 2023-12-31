#pragma once
#include "Character.h"
#include "../RigidBodies/BoxStaticRB.h"
#include <list>

class Scene;

class CollisionManager {
private:
	Scene* scene;

	std::pair<Character*, StaticRigidBody*> characterCol;
	vector<BoxStaticRB*> platforms;

	void collisionCharacterPlatforms(PxActor* actor1, PxActor* actor2);

	bool collisionCharPlatform(PxActor* actor, PxActor* actorBis);

public:
	CollisionManager(Character* character, vector<BoxStaticRB*> platforms);

	void onCollision(physx::PxActor* actor1, physx::PxActor* actor2);

	void integrate(double t);
};