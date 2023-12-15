#pragma once
#include "DynamicRigidBody.h"

class BoxDynamicRB : public DynamicRigidBody {
private:

public:
	BoxDynamicRB(PxPhysics* gPhysics, PxScene* gScene, const Vector3& pos, const Vector3& linearVel,
		const Vector3& angularVel, float damping, float density, Vector4 color, Vector3 size,
		PxMaterial* material = nullptr, float lifeTime = -1) :
		DynamicRigidBody(gPhysics, gScene, pos, linearVel, angularVel, damping, density, color,
			// se pasa dir de memoria para que se pueda pasar el hijo (virtual)
			&PxBoxGeometry(size * 2), material, lifeTime) {}

	BoxDynamicRB(PxPhysics* gPhysics, PxScene* gScene, const Vector3& pos, const Vector3& linearVel,
		const Vector3& angularVel, float damping, Vector3 massDistribution, Vector4 color, Vector3 size,
		PxMaterial* material = nullptr, float lifeTime = -1) :
		DynamicRigidBody(gPhysics, gScene, pos, linearVel, angularVel, damping, massDistribution, color,
			&PxBoxGeometry(size * 2), material, lifeTime) {}
};