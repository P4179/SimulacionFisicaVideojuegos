#pragma once
#include "RenderUtils.hpp"

class Particle {
private:
	// coeficiente damping -> se utiliza para solucionar errores de simulación
	// se puede utilizar además para simular rozamiento
	const double DAMPING;
	const float GRAVEDAD_REAL = 500;
	const float TIMER_ALIVE = 10;

	// vector velocidad, que indica la dirección del tiro
	// tiene en cuenta la velocidad simulada
	Vector3 vel;
	// a render item le pasaremos la dirección de este pose, para que se actualize automáticamente
	physx::PxTransform pose;
	RenderItem* renderItem;

	float masaReal;
	float masaSimulada;
	// se reduce la velocidad para que se pueda percibir la trayectoria de tiro de la bola
	float vReal;
	float vSimulada;
	float gravedadSimulada;

	bool alive;
	float elapsedTime;

public:
	Particle(Vector3 pos, Vector3 vel, double damping, float masaReal, float vReal, float vSimulada) :
		pose(pos.x, pos.y, pos.z), vel(vel), DAMPING(damping), masaReal(masaReal), vReal(vReal), vSimulada(vSimulada),
		renderItem(nullptr), alive(true), elapsedTime(0) {
		// se necesita un radio
		physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(2.0f));
		//  geometría, posición, color (R, G, B, alpha)
		renderItem = new RenderItem(shape, &pose, Vector4(1, 0, 0, 1));
		RegisterRenderItem(renderItem);

		// como se ha reducido la velocidad, para conseguir que la energía cinética sea mayor,
		// se utiliza una masa diferente a la real
		// a mayor energía cinética, mayor es el impacto
		masaSimulada = masaReal * (vReal / vSimulada) * (vReal / vSimulada);
		// como se ha reducido la velocidad, se utiliza una gravedad simulada
		// para que el lanzamiento no parezca el de un globo
		// los únicos parámetros que se van a varíar para aumentar la velocidad del tiro
		// son gravedad y aceleración
		gravedadSimulada = GRAVEDAD_REAL * (vSimulada / vReal) * (vSimulada / vReal);
	}

	virtual ~Particle() {
		DeregisterRenderItem(renderItem);
	}

	void integrate(double t) {
		// MRU
		// se modifica el vector3 de pose
		//pose.p += vel * t;

		// MRUA
		vel += Vector3(0, -gravedadSimulada, 0) * t;
		// rozamiento
		vel *= pow(DAMPING, t);
		pose.p += vel * t;

		elapsedTime += t;
		if (elapsedTime > TIMER_ALIVE) {
			elapsedTime = 0;
			alive = false;
		}
	}

	bool getAlive() const {
		return alive;
	}
};