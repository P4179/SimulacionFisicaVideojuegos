#pragma once
#include "./ForceGenerators/ForceGenerator.h"
#include "Particles/Particle.h"
#include <unordered_set>
#include <unordered_map>

class ParticleForceRegistry {
private:
	// se hace de esta manera para mejorar la complejidad
	// tanto para eliminar una partícula como una fuerza se puede hacer directamente
	// por ejemplo, para eliminar una fuerza hay que ir recorriendo las partículas y se puede
	// comprobar directamente si esa partícula tiene asociada esa fuerza 
	// (no hace falta recorrer todas las fuerzas)

	// las fuerzas que se le asignan a cada partícula
	unordered_map<Particle*, unordered_set<ForceGenerator*>> particleForce;
	// las partículas que le corresponden a cada fuerza
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