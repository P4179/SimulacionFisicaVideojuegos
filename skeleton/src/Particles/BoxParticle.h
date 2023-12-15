#pragma once
#include "Particle.h"

class BoxParticle : public Particle {
private:
	Vector3 size;
	float volume;

public:
	BoxParticle(Vector3 pos, Vector3 vel, float invMasa, double damping, float lifeTime, float vSimulada, Vector3 size, Vector4 color = Vector4(1, 0, 0, 1)) :
		Particle(pos, vel, invMasa, damping, lifeTime, vSimulada, &physx::PxBoxGeometry(size), color), size(size), volume(size.x* size.y* size.z) {}

	// FLOTACION PARA CAJAS
	virtual inline void disminuirTam() {
		Vector3 aux = size + Vector3(-2);
		if (aux.x > 0 && aux.y > 0 && aux.z > 0) {
			size = aux;
			volume = size.x * size.y * size.z;
			DeregisterRenderItem(renderItem);
			physx::PxShape* shape = CreateShape(physx::PxBoxGeometry(size));
			renderItem = new RenderItem(shape, &pose, color);
		}
	}
	virtual inline void aumentarTam() {
		size += Vector3(2);
		volume = size.x * size.y * size.z;
		DeregisterRenderItem(renderItem);
		physx::PxShape* shape = CreateShape(physx::PxBoxGeometry(size));
		renderItem = new RenderItem(shape, &pose, color);
	}
	virtual inline float getLength() const {
		return size.y;
	}
	virtual inline float getVolume() const {
		return volume;
	}

	// VIENTO PARA CAJAS
	virtual inline float getArea() const {
		return 2 * size.x * size.y + 2 * size.z * size.y + 2 * size.x * size.z;
	}
};