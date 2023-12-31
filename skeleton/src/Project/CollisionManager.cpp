#include "CollisionManager.h"
#include "Scene.h"

void CollisionManager::collisionCharacterPlatforms(PxActor* actor1, PxActor* actor2) {
	if (!collisionCharPlatform(actor1, actor2)) {
		collisionCharPlatform(actor2, actor1);
	}
}

bool CollisionManager::collisionCharPlatform(PxActor* actor, PxActor* actorBis) {
	bool isChar = characterCol.first->getActor() == actor;
	if (isChar) {
		auto it = std::find_if(platforms.begin(), platforms.end(), [actorBis](BoxStaticRB* plat) {
			return plat->getActor() == actorBis;
			});
		if (it != platforms.end()) {
			characterCol.second = *it;

			if (characterCol.first->detectOnFloor(*it)) {
				scene->processContactPlatform(*it);
			}
		}
	}
	return isChar;
}

CollisionManager::CollisionManager(Character* character, vector<BoxStaticRB*> platforms) :
	characterCol({ character, nullptr }), platforms(platforms) {
	
	scene = Scene::get();
}

void CollisionManager::onCollision(physx::PxActor* actor1, physx::PxActor* actor2) {
	collisionCharacterPlatforms(actor1, actor2);
}

void CollisionManager::integrate(double t) {
	characterCol.first->collision(characterCol.second);

	characterCol.second = nullptr;
}