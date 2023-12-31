#include "Particle.h"
#include <iostream>
#include "../checkML.h"

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

// PRACTICA 2
// NO SE UTILIZA: INVERSO DE LA MASA
Particle::Particle(Vector3 pos, Vector3 vel, Vector3 acReal, double damping, float lifeTime, float vSimulada, float radius, Vector4 color, ParticleType type) :
	pose(pos.x, pos.y, pos.z), vel(vel), damping(damping), lifeTime(lifeTime), renderItem(nullptr), alive(true), elapsedTime(0), radius(radius), color(color),
	immersion(Out) {
	// se necesita un radio
	physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(radius));
	commonConstructor(shape, color, vSimulada);

	// se utiliza directamente la ACELERACION
	acSimulada = acReal;

	// PRACTICA 1
	//calculateSimulatedPhysics(type, acReal, vSimulada);
}

// PRACTICA 3 EN ADELANTE (CREAR ESFERA)
// NO SE UTILIZA: ACELERACION SIMULADA, MASA SIMULADA
Particle::Particle(Vector3 pos, Vector3 vel, float invMasa, double damping, float lifeTime, float vSimulada, float radius, Vector4 color) :
	pose(pos.x, pos.y, pos.z), vel(vel), invMasa(invMasa), damping(damping), lifeTime(lifeTime), renderItem(nullptr), alive(true),
	elapsedTime(0), force(Vector3(0)), radius(radius), color(color), immersion(Out) {
	// se necesita un radio
	physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(radius));
	commonConstructor(shape, color, vSimulada);
}

// CREAR CUALQUIER FORMA
// NO SE UTILIZA: ACELERACION SIMULADA, MASA SIMULADA, RADIO
Particle::Particle(Vector3 pos, Vector3 vel, float invMasa, double damping, float lifeTime, float vSimulada, physx::PxGeometry* geometry, Vector4 color) :
	pose(pos.x, pos.y, pos.z), vel(vel), invMasa(invMasa), damping(damping), lifeTime(lifeTime), renderItem(nullptr), alive(true),
	elapsedTime(0), force(Vector3(0)), color(color), immersion(Out) {

	if (geometry != nullptr) {
		physx::PxShape* shape = CreateShape(*geometry);
		commonConstructor(shape, color, vSimulada);
	}
}

Particle::~Particle() {
	if (renderItem != nullptr) {
		DeregisterRenderItem(renderItem);
	}
}

// t está en segundos
void Particle::updateLifeTime(double t) {
	if (lifeTime >= 0) {
		elapsedTime += t;
		if (elapsedTime > lifeTime) {
			elapsedTime = 0;
			alive = false;
		}
	}
}

void Particle::integrate(double t) {
	// MRU
	// se modifica el vector3 de pose
	//pose.p += vel * t;

	// MRUA
	// PRACTICA 1 --> vel += Vector3(0, -gravedadSimulada, 0) * t;
	
	// PRACTICA 2 --> vel += acSimulada * t;
	
	// PRACTICA 3
	// Segunda Ley de Newton --> F=m*a
	// Por lo tanto, a = F/m
	// Para evitar errores de compilacion, es decir, que se aplique
	// una fuerza a un objeto de masa 0 se utiliza directamente el
	// inverso de la masa (a = F*invMasa)
	try {
		// ALGORITMO DE EULER SEMI-IMPLÍCITO (PRIMERO VELOCIDAD, LUEGO POS)
		Vector3 resultAc = force * getInvMasa();
		vel += resultAc * t;

		// rozamiento
		vel *= pow(damping, t);
		pose.p += vel * t;
	}
	catch (std::exception e) {
		std::cout << "Could not apply force to this particle: " << e.what() << "\n";
	}

	updateLifeTime(t);

	// eliminar las fuerzas
	clearForce();
}