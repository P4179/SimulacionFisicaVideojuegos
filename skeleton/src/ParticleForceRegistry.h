#pragma once
#include "./ForceGenerators/ForceGenerator.h"
#include "Particles/Particle.h"
#include <unordered_set>
#include <unordered_map>

class ParticleForceRegistry {
private:
	// se hace de esta manera para mejorar la complejidad
	// tanto para eliminar una part�cula como una fuerza se puede hacer directamente
	// por ejemplo, para eliminar una fuerza hay que ir recorriendo las part�culas y se puede
	// comprobar directamente si esa part�cula tiene asociada esa fuerza 
	// (no hace falta recorrer todas las fuerzas)

	// las fuerzas que se le asignan a cada part�cula
	unordered_map<Particle*, unordered_set<ForceGenerator*>> particleForce;
	// las part�culas que le corresponden a cada fuerza
	unordered_map<ForceGenerator*, unordered_set<Particle*>> forceParticle;

public:
	ParticleForceRegistry();

	void updateForces(double t);

	void addRegistry(ForceGenerator* fg, Particle* p);

	void deleteParticleRegistry(Particle* particle);

	void deleteForceRegistry(ForceGenerator* fg);

	void updateTime(double t);

	inline void clear() {
		particleForce.clear();
		forceParticle.clear();
	}
};