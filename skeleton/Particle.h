#pragma once
#include "RenderUtils.hpp"

class Particle {
private:
	Vector3 ac;
	Vector3 vel;
	// a render item le pasaremos la dirección de este pose, para que se actualize automáticamente
	physx::PxTransform pose;
	RenderItem* renderItem;

public:
	Particle(Vector3 Pos, Vector3 Ac) : ac(Ac), vel(0, 0, 0), pose(Pos.x, Pos.y, Pos.z), renderItem(nullptr) {
		// se necesita un radio
		physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(2.0f));
		//  geometría, posición, color (R, G, B, alpha)
		renderItem = new RenderItem(shape, &pose, Vector4(1, 0, 0, 1));
		RegisterRenderItem(renderItem);
	}

	virtual ~Particle() {
		DeregisterRenderItem(renderItem);
	}

	void integrate(double t) {
		// MRU
		// se modifica el vector3 del pose
		// se multiplica por deltaTime parea que vaya de acuerdo a los frames
		//pose.p += vel * t;

		// MRUA
		vel += ac * t;
		// se puede omitir a partir del primer t
		pose.p += vel * t + 1 / 2 * ac * (t * t);
	}
};