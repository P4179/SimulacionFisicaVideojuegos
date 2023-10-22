#pragma once
#include "../../RenderUtils.hpp"
#include <iostream>
#include <list>

enum ParticleType { Default };

class Particle {
private:
	// coeficiente damping -> se utiliza para solucionar errores de simulación
	// se puede utilizar además para simular rozamiento
	double damping;

	// vector velocidad, que indica la dirección del tiro
	// luego, tiene en cuenta la velocidad simulada
	Vector3 vel;
	// a render item le pasaremos la dirección de este pose, para que se actualize automáticamente
	physx::PxTransform pose;
	RenderItem* renderItem;

	float masaSimulada;
	Vector3 acSimulada;

	// tiempo de vida de la partícula
	float lifeTime;
	bool alive;
	float elapsedTime;

	// se utiliza para clonar la partícula
	float vSimulada;
	ParticleType type;
	float radius;
	Vector4 color;

	// t está en segundos
	void updateLifeTime(double t) {
		elapsedTime += t;
		if (elapsedTime > lifeTime) {
			elapsedTime = 0;
			alive = false;
		}
	}

	void infoParticleType(ParticleType type, float& masaReal, float& vReal) {
		switch (type) {
		case Default:
			masaReal = 300, vReal = 500;
			break;
		}
	}

public:
	Particle(Vector3 pos, Vector3 vel, Vector3 acReal, double damping, float lifeTime, float vSimulada, float radius = 2, Vector4 color = Vector4(1, 0, 0, 1), ParticleType type = Default) :
		pose(pos.x, pos.y, pos.z), vel(vel), damping(damping), lifeTime(lifeTime), renderItem(nullptr), alive(true), elapsedTime(0),
		vSimulada(vSimulada), type(type), radius(radius), color(color) {
		// se necesita un radio
		physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(radius));
		// geometría, posición, color (R, G, B, alpha)
		// RGB va desde 0 hasta 1
		renderItem = new RenderItem(shape, &pose, color);
		RegisterRenderItem(renderItem);

		float masaReal = 0, vReal = 0;
		// dependiendo del tipo de bala se le asigna una masa real y una velocidad real diferentes
		infoParticleType(type, masaReal, vReal);

		// velocidad
		vel = vel.getNormalized();
		this->vel = vel * vSimulada;

		// como se ha reducido la velocidad, para conseguir que la energía cinética sea mayor,
		// se utiliza una masa diferente a la real
		// a mayor energía cinética, mayor es el impacto
		masaSimulada = masaReal * (vReal / vSimulada) * (vReal / vSimulada);

		// como se ha reducido la velocidad, se utiliza una aceleracion (gravedad) simulada
		// para que el proyectil no se caiga para abajo de repente
		// sin embargo, se podría asignar la que queramos
		// para los proyectiles se calcula la simulada a partir de la real
		// pero para el sistema de partículas se utiliza la real

		acSimulada = acReal; //* (vSimulada / vReal) * (vSimulada / vReal);
	}

	virtual ~Particle() {
		DeregisterRenderItem(renderItem);
	}

	void integrate(double t) {
		// MRU
		// se modifica el vector3 de pose
		//pose.p += vel * t;

		// MRUA
		vel += acSimulada * t; //Vector3(0, -gravedadSimulada, 0) * t;
		// rozamiento
		vel *= pow(damping, t);
		pose.p += vel * t;

		updateLifeTime(t);
	}

	bool isAlive() const {
		return alive;
	}

	void setAlive(bool alive) {
		this->alive = alive;
	}

	virtual void onDeath(std::list<Particle*>& particles) {};

	// NO SE USA
	virtual Particle* clone() const {
		// como se va a normalizar no pasa nada por pasar la velocidad multiplicada por vSimulada
		// clone se utiliza para clonar las partículas que se van a meter en la lista de partículas
		// por lo tanto, se borrarán en el update del sistema de partículas
		Particle* newParticle = new Particle(pose.p, vel, acSimulada, damping, lifeTime, vSimulada, radius, color, type);
		return newParticle;
	}

	inline Vector3 getPos() const {
		return pose.p;
	}

	inline Vector3 setPos(Vector3 pos) {
		pose.p = pos;
	}

	inline Vector3 getVel() const {
		return vel;
	}

	inline void setLifeTime(float lifeTime) {
		this->lifeTime = lifeTime;
	}
};