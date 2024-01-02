#pragma once
#include "../RigidBodies/DynamicRigidBody.h"

class CapsuleDynamicRB : public DynamicRigidBody {
private:
	float radius;
	float height;

public:
	CapsuleDynamicRB(PxPhysics* gPhysics, PxScene* gScene, const Vector3& pos, const Vector3& linearVel,
		const Vector3& angularVel, float damping, float density, Vector4 color, float radius, float height,
		PxMaterial* material = nullptr, float lifeTime = -1, bool render = true) :
		DynamicRigidBody(gPhysics, gScene, pos, linearVel, angularVel, damping, density, color,
			// se pasa dir de memoria para que se pueda pasar el hijo (virtual)
			&PxCapsuleGeometry(radius, height / 2), material, lifeTime, render), radius(radius), height(height) {}

	CapsuleDynamicRB(PxPhysics* gPhysics, PxScene* gScene, const Vector3& pos, const Vector3& linearVel,
		const Vector3& angularVel, float damping, Vector3 massDistribution, Vector4 color, float radius, float height,
		PxMaterial* material = nullptr, float lifeTime = -1, bool render = true) :
		DynamicRigidBody(gPhysics, gScene, pos, linearVel, angularVel, damping, massDistribution, color,
			&PxCapsuleGeometry(radius, height / 2), material, lifeTime, render), radius(radius), height(height) {}

	// TODO SE SIMULA COMO SI FUERAN CAJAS
	
	// COLISONES
	// para las detectar si el personaje esta en el suelo de las plataformas
	inline float getFloorY() const override {
		float posY = getPos().y;
		return posY - (height / 2) - radius;
	}

	// FLOTACION PARA CAPSULAS
	inline float getLength() const override {
		// hack: trucar la flotacion
		return (height + radius * 2) / 3;
	}
	inline float getVolume() const override {
		// hack: trucar la flotacion
		return (radius * radius * height) / 3;
	}

	// VIENTO PARA CAPSULAS
	inline float getArea() const override {
		return 2 * radius * height + 2 * radius * height + 2 * radius * radius;
	}
};
