#include "Particle.h"
#include "../checkML.h"

// t está en segundos
void Particle::updateLifeTime(double t) {
	elapsedTime += t;
	if (elapsedTime > lifeTime) {
		elapsedTime = 0;
		alive = false;
	}
}

void Particle::infoParticleType(ParticleType type, float& masaReal, float& vReal) {
	switch (type) {
	case Default:
		masaReal = 300, vReal = 500;
		break;
	}
}

void Particle::calculateSimulatedPhysics(ParticleType type, Vector3 acReal, float vSimulada) {
	float masaReal = 0, vReal = 0;
	// dependiendo del tipo de bala se le asigna una masa real y una velocidad real diferentes
	infoParticleType(type, masaReal, vReal);

	// VELOCIDAD SIMULADA
	vel = vel.getNormalized();
	this->vel = vel * vSimulada;

	// MASA SIMULADA
	// como se ha reducido la velocidad, para conseguir que la energía cinética sea mayor,
	// se utiliza una masa diferente a la real
	// a mayor energía cinética, mayor es el impacto
	masaSimulada = masaReal * (vReal / vSimulada) * (vReal / vSimulada);

	// Como se ha reducido la velocidad, se utiliza una aceleracion (gravedad) simulada
	// para que el proyectil no se caiga para abajo de repente
	// Para los proyectiles se calcula la simulada a partir de la real,
	// pero para el sistema de partículas se utiliza la real
	acSimulada = acReal * (vSimulada / vReal) * (vSimulada / vReal);
}

Particle::Particle(Vector3 pos, Vector3 vel, Vector3 acReal, double damping, float lifeTime, float vSimulada, float radius, Vector4 color, ParticleType type) :
	pose(pos.x, pos.y, pos.z), vel(vel), damping(damping), lifeTime(lifeTime), renderItem(nullptr), alive(true), elapsedTime(0) {
	// se necesita un radio
	physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(radius));
	// geometría, posición, color (R, G, B, alpha)
	// RGB va desde 0 hasta 1
	// el new ya hace el register
	renderItem = new RenderItem(shape, &pose, color);

	// VELOCIDAD
	// no esta mal usar la velocidad simulada
	// es decir, vel indciar la dir y vSimulada la magnitud del vector
	// sin embargo, estaria mejor hace que vel sea el propio vector e incluya todo
	vel = vel.getNormalized();
	this->vel = vel * vSimulada;

	// se utiliza directamente la ACELERACION
	acSimulada = acReal;

	// PRACTICA 1
	//calculateSimulatedPhysics(type, acReal, vSimulada);
}

Particle::~Particle() {
	DeregisterRenderItem(renderItem);
	//delete renderItem;
}

void Particle::integrate(double t) {
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