#pragma once
#include <PxPhysicsAPI.h>
#include "../../RenderUtils.hpp"

using namespace physx;

// Elementos fijos en la escena
class StaticRigidBody {
protected:
	PxPhysics* gPhysics;
	PxScene* gScene;
	RenderItem* renderItem;
	PxRigidStatic* staticRigid;

	StaticRigidBody(PxPhysics* gPhysics, PxScene* gScene, const Vector3& pos, const Vector4& color, PxShape* shape) :
		gPhysics(gPhysics), gScene(gScene) {

		// fisicas y posicion del objeto estatico
		staticRigid = gPhysics->createRigidStatic(PxTransform({ pos }));
		staticRigid->attachShape(*shape);
		// se pone el elem en la escena, pero luego hay que pintarlo aparte
		gScene->addActor(*staticRigid);

		// se pinta el elem
		renderItem = new RenderItem(shape, staticRigid, color);
	}

public:
	virtual ~StaticRigidBody() {
		//DeregisterRenderItem(renderItem);
	}

	inline PxRigidActor* getActor() const {
		return staticRigid;
	}

	Vector3 getPos() const {
		return staticRigid->getGlobalPose().p;
	}

	// COLISIONES
	virtual float getCeilingY() const = 0;
	
};