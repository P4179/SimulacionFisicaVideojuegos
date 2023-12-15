#pragma once
#include "DynamicRigidBody.h"

class SphereDynamicRB : public DynamicRigidBody {
private:

public:
	SphereDynamicRB(PxPhysics* gPhysics, PxScene* gScene, const Vector3& pos, const Vector3& linearVel,
		const Vector3& angularVel, float damping, float density, Vector4 color, float radius,
		PxMaterial* material = nullptr, float lifeTime = -1) :
		DynamicRigidBody(gPhysics, gScene, pos, linearVel, angularVel, damping, density, color,
			// se pasa dir de memoria para que se pueda pasar el hijo (virtual)
			&PxSphereGeometry(radius), material, lifeTime) {}

	SphereDynamicRB(PxPhysics* gPhysics, PxScene* gScene, const Vector3& pos, const Vector3& linearVel,
		const Vector3& angularVel, float damping, Vector3 massDistribution, Vector4 color, float radius,
		PxMaterial* material = nullptr, float lifeTime = -1) :
		DynamicRigidBody(gPhysics, gScene, pos, linearVel, angularVel, damping, massDistribution, color,
			&PxSphereGeometry(radius)) {}
};