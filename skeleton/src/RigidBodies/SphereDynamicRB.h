#pragma once
#include "DynamicRigidBody.h"

class SphereDynamicRB : public DynamicRigidBody {
private:

public:
	SphereDynamicRB(PxPhysics* gPhysics, PxScene* gScene, const Vector3& pos, const Vector3& linearVel,
		const Vector3& angularVel, float damping, float density, Vector4 color, float lifeTime, float radius) :
		DynamicRigidBody(gPhysics, gScene, pos, linearVel, angularVel, damping, density, color,
			CreateShape(PxSphereGeometry(radius)), lifeTime) {}

	SphereDynamicRB(PxPhysics* gPhysics, PxScene* gScene, const Vector3& pos, const Vector3& linearVel,
		const Vector3& angularVel, float damping, Vector3 massDistribution, Vector4 color, float lifeTime, float radius) :
		DynamicRigidBody(gPhysics, gScene, pos, linearVel, angularVel, damping, massDistribution, color,
			CreateShape(PxSphereGeometry(radius)), lifeTime) {}
};