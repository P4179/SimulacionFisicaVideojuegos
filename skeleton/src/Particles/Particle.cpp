#include "Particle.h"
#include <iostream>

// t est� en segundos
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
	// como se ha reducido la velocidad, para conseguir que la energ�a cin�tica sea mayor,
	// se utiliza una masa diferente a la real
	// a mayor energ�a cin�tica, mayor es el impacto
	masaSimulada = masaReal * (vReal / vSimulada) * (vReal / vSimulada);

	// Como se ha reducido la velocidad, se utiliza una aceleracion (gravedad) simulada
	// para que el proyectil no se caiga para abajo de repente
	// Para los proyectiles se calcula la simulada a partir de la real,
	// pero para el sistema de part�culas se utiliza la real
	acSimulada = acReal * (vSimulada / vReal) * (vSimulada / vReal);
}

Particle::Particle(Vector3 pos, Vector3 vel, Vector3 acReal, double damping, float lifeTime, float vSimulada, float radius, Vector4 color, ParticleType type) :
	pose(pos.x, pos.y, pos.z), vel(vel), damping(damping), lifeTime(lifeTime), renderItem(nullptr), alive(true), elapsedTime(0), radius(radius) {
	// se necesita un radio
	physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(radius));
	// geometr�a, posici�n, color (R, G, B, alpha)
	// RGB va desde 0 hasta 1
	// el new ya hace el register
	renderItem = new RenderItem(shape, &pose, color);

	// VELOCIDAD
	// no esta mal usar la velocidad simulada
	// es decir, vel indica la dir y vSimulada la magnitud del vector
	// sin embargo, estaria mejor hace que vel sea el propio vector e incluya todo
	vel = vel.getNormalized();
	this->vel = vel * vSimulada;

	// se utiliza directamente la ACELERACION
	acSimulada = acReal;

	// PRACTICA 1
	//calculateSimulatedPhysics(type, acReal, vSimulada);
}

Particle::Particle(Vector3 pos, Vector3 vel, float invMasa, double damping, float lifeTime, float vSimulada, float radius, Vector4 color) :
	pose(pos.x, pos.y, pos.z), vel(vel), invMasa(invMasa), damping(damping), lifeTime(lifeTime), renderItem(nullptr), alive(true),
	elapsedTime(0), force(Vector3(0)), radius(radius) {
	// se necesita un radio
	physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(radius));
	// geometr�a, posici�n, color (R, G, B, alpha)
	// RGB va desde 0 hasta 1
	// el new ya hace el register
	renderItem = new RenderItem(shape, &pose, color);

	// VELOCIDAD
	// no esta mal usar la velocidad simulada
	// es decir, vel indica la dir y vSimulada la magnitud del vector
	// sin embargo, estaria mejor hace que vel sea el propio vector e incluya todo
	vel = vel.getNormalized();
	this->vel = vel * vSimulada;
}

Particle::~Particle() {
	DeregisterRenderItem(renderItem);
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