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

	// NO SE USA
	// se utiliza para clonar la part�cula
	float vSimulada;
	ParticleType type;
	float radius;
	Vector4 color;

	// t est� en segundos
	void updateLifeTime(double t);

	void infoParticleType(ParticleType type, float& masaReal, float& vReal);

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

	// NO SE USA
	inline virtual Particle* clone() const {
		// como se va a normalizar no pasa nada por pasar la velocidad multiplicada por vSimulada
		// clone se utiliza para clonar las part�culas que se van a meter en la lista de part�culas
		// por lo tanto, se borrar�n en el update del sistema de part�culas
		Particle* newParticle = new Particle(pose.p, vel, acSimulada, damping, lifeTime, vSimulada, radius, color, type);
		return newParticle;
	}

	inline Vector3 getPos() const {
		return pose.p;
	}

	inline void setPos(Vector3 pos) {
		pose.p = pos;
	}

	inline Vector3 getVel() const {
		return vel;
	}

	inline void setLifeTime(float lifeTime) {
		this->lifeTime = lifeTime;
	}
};