#pragma once
#include "StaticRigidBody.h"

class BoxStaticRB : public StaticRigidBody {
private:
	Vector3 size;

public:
	BoxStaticRB(PxPhysics* gPhysics, PxScene* gScene, const Vector3& pos, const Vector4& color, const Vector3& size, PxMaterial* material = nullptr) :
		StaticRigidBody(gPhysics, gScene, pos, color, CreateShape(PxBoxGeometry(size / 2), material)), size(size) { }

	float getCeilingY() const override {
		float posY = getPos().y;
		return posY + (size.y / 2);
	}
};