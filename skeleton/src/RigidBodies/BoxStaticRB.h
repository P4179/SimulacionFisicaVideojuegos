#pragma once
#include "StaticRigidBody.h"

class BoxStaticRB : public StaticRigidBody {
private:

public:
	BoxStaticRB(PxPhysics* gPhysics, PxScene* gScene, const Vector3& pos, const Vector4& color, const Vector3& size) :
		StaticRigidBody(gPhysics, gScene, pos, color, CreateShape(PxBoxGeometry(size * 2))) { }
};