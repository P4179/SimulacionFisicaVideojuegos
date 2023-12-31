#pragma once
#include "DynamicRigidBody.h"

class SphereDynamicRB : public DynamicRigidBody {
private:
	// la particula esfera tb tiene un atb radio, pero no tiene sentido
	// que el rigid body esfera accede a el porque rigidbody es una clase "limpia"
	float radius;

public:
	SphereDynamicRB(PxPhysics* gPhysics, PxScene* gScene, const Vector3& pos, const Vector3& linearVel,
		const Vector3& angularVel, float damping, float density, Vector4 color, float radius,
		PxMaterial* material = nullptr, float lifeTime = -1, bool render = true) :
		DynamicRigidBody(gPhysics, gScene, pos, linearVel, angularVel, damping, density, color,
			// se pasa dir de memoria para que se pueda pasar el hijo (virtual)
			&PxSphereGeometry(radius), material, lifeTime, render), radius(radius) {}

	SphereDynamicRB(PxPhysics* gPhysics, PxScene* gScene, const Vector3& pos, const Vector3& linearVel,
		const Vector3& angularVel, float damping, Vector3 massDistribution, Vector4 color, float radius,
		PxMaterial* material = nullptr, float lifeTime = -1, bool render = true) :
		DynamicRigidBody(gPhysics, gScene, pos, linearVel, angularVel, damping, massDistribution, color,
			&PxSphereGeometry(radius), material, lifeTime, render), radius(radius) {}

	// COLISIONES
	// para las detectar si el personaje esta en el suelo de las plataformas
	inline float getFloorY() const override {
		return getPos().y - radius;
	}

	// NO TIENE FLOTACION

	// VIENTO PARA ESFERAS
	inline float getArea() const override {
		return 4 * radius * physx::PxPi;
	}
};