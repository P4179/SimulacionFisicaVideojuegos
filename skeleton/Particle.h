#pragma once
#include "RenderUtils.hpp"

class Particle {
private:
	enum ParticleType { Gun };

	// coeficiente damping -> se utiliza para solucionar errores de simulación
	// se puede utilizar además para simular rozamiento
	const double DAMPING;

	// a simple vista no se aprecia porque tiene que recorrer mucho espacio para comenzar a caerse
	//const float GRAVEDAD_REAL = 10;
	const float LIFE_TIME;

	// vector velocidad, que indica la dirección del tiro
	// tiene en cuenta la velocidad simulada
	Vector3 vel;
	// a render item le pasaremos la dirección de este pose, para que se actualize automáticamente
	physx::PxTransform pose;
	RenderItem* renderItem;

	float masaSimulada;
	// se reduce la velocidad para que se pueda percibir la trayectoria de tiro de la bola
	//float gravedadSimulada;
	Vector3 acSimulada;

	// tiempo de vida de la partícula
	bool alive;
	float elapsedTime;

	void updateLifeTime(double t) {
		elapsedTime += t;
		if (elapsedTime > LIFE_TIME) {
			elapsedTime = 0;
			alive = false;
		}
	}

public:
	Particle(Vector3 pos, Vector3 vel, Vector3 acReal, double damping, float lifeTime, float vSimulada = 25, float radius = 2, Vector4 color = Vector4(1, 0, 0, 1), ParticleType type = Gun) :
		pose(pos.x, pos.y, pos.z), vel(vel), DAMPING(damping), LIFE_TIME(lifeTime), renderItem(nullptr), alive(true), elapsedTime(0) {
		// se necesita un radio
		physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(radius));
		//  geometría, posición, color (R, G, B, alpha)
		renderItem = new RenderItem(shape, &pose, color);
		RegisterRenderItem(renderItem);

		float masaReal = 0, vReal = 0;
		// dependiendo del tipo de bala se le asigna una masa real y una velocidad real diferentes
		if (type == Gun) {
			masaReal = 300;
			vReal = 500;
		}
		vel = vel.getNormalized() * vSimulada;

		// como se ha reducido la velocidad, para conseguir que la energía cinética sea mayor,
		// se utiliza una masa diferente a la real
		// a mayor energía cinética, mayor es el impacto
		masaSimulada = masaReal * (vReal / vSimulada) * (vReal / vSimulada);
		// como se ha reducido la velocidad, se utiliza una gravedad simulada
		// para que el lanzamiento no parezca el de un globo
		// los únicos parámetros que se van a varíar para aumentar la velocidad del tiro
		// son gravedad y aceleración

		// se podría asignar la que queramos
		// dependiendo del tipo de bala, se cambia la gravedad
		acSimulada = acReal * (vSimulada / vReal) * (vSimulada / vReal);
		// gravedadSimulada = GRAVEDAD_REAL * (vSimulada / vReal) * (vSimulada / vReal);
	}

	virtual ~Particle() {
		DeregisterRenderItem(renderItem);
	}

	void integrate(double t) {
		// MRU
		// se modifica el vector3 de pose
		//pose.p += vel * t;

		// MRUA
		vel += acSimulada * t;//Vector3(0, -gravedadSimulada, 0) * t;
		// rozamiento
		vel *= pow(DAMPING, t);
		pose.p += vel * t;

		updateLifeTime(t);
	}

	bool isAlive() const {
		return alive;
	}
};