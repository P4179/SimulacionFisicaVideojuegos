#pragma once
#include "../../RenderUtils.hpp"

class ListParticles;

enum ParticleType { Default };

class Particle {
protected:
	// coeficiente damping -> se utiliza para solucionar errores de simulaci�n
	// se puede utilizar adem�s para simular rozamiento
	double damping;

	// vector velocidad, que indica la direcci�n del tiro
	// luego, tiene en cuenta la velocidad simulada
	Vector3 vel;
	// a render item le pasaremos la direcci�n de este pose, para que se actualize autom�ticamente
	physx::PxTransform pose;
	RenderItem* renderItem;

	float masaSimulada;
	Vector3 acSimulada;

	// tiempo de vida de la part�cula
	float lifeTime;
	bool alive;
	float elapsedTime;

	// t est� en segundos
	void updateLifeTime(double t);

	void infoParticleType(ParticleType type, float& masaReal, float& vReal);

	// PRACTICA 1
	// type --> velocidad y masas reales
	void calculateSimulatedPhysics(ParticleType type, Vector3 acReal, float vSimulada);

public:
	Particle(Vector3 pos, Vector3 vel, Vector3 acReal, double damping, float lifeTime, float vSimulada, float radius = 2, Vector4 color = Vector4(1, 0, 0, 1), ParticleType type = Default);

	virtual ~Particle();

	void integrate(double t);

	inline bool isAlive() const {
		return alive;
	}

	inline void setAlive(bool alive) {
		this->alive = alive;
	}

	inline bool getAlive() const {
		return alive;
	}

	virtual void onDeath(ListParticles* particles) {};
};