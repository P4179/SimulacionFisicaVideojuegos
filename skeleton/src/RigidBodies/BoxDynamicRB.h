#pragma once
#include "DynamicRigidBody.h"

class BoxDynamicRB : public DynamicRigidBody {
private:
	Vector3 size;
	float volume;

public:
	BoxDynamicRB(PxPhysics* gPhysics, PxScene* gScene, const Vector3& pos, const Vector3& linearVel,
		const Vector3& angularVel, float damping, float density, Vector4 color, Vector3 size,
		PxMaterial* material = nullptr, float lifeTime = -1, bool render = true) :
		DynamicRigidBody(gPhysics, gScene, pos, linearVel, angularVel, damping, density, color,
			// se pasa dir de memoria para que se pueda pasar el hijo (virtual)
			&PxBoxGeometry(size), material, lifeTime, render), size(size), volume(size.x* size.y* size.z) {}

	BoxDynamicRB(PxPhysics* gPhysics, PxScene* gScene, const Vector3& pos, const Vector3& linearVel,
		const Vector3& angularVel, float damping, Vector3 massDistribution, Vector4 color, Vector3 size,
		PxMaterial* material = nullptr, float lifeTime = -1, bool render = true) :
		DynamicRigidBody(gPhysics, gScene, pos, linearVel, angularVel, damping, massDistribution, color,
			&PxBoxGeometry(size), material, lifeTime, render), size(size), volume(size.x* size.y* size.z) {}

	// COLISONES
	// para las detectar si el personaje esta en el suelo de las plataformas
	inline float getFloorY() const override {
		return getPos().y - size.y / 2;
	}

	// FLOTACION PARA CAJAS
	inline float getLength() const override {
		return size.y;
	}
	inline float getVolume() const override {
		return volume;
	}

	// VIENTO PARA CAJAS
	inline float getArea() const override {
		return 2 * size.x * size.y + 2 * size.z * size.y + 2 * size.x * size.z;
	}
};