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
	ParticleForceRegistry() : particleForce(), forceParticle() {}

	void updateForces(double t) {
		for (auto it = forceParticle.begin(); it != forceParticle.end(); ++it) {
			auto particles = it->second;
			for (auto particle : particles) {
				it->first->updateForce(particle, t);
			}
		}
	}

	void addRegistry(ForceGenerator* fg, Particle* p) {
		// SISTEMA PARTÍCULA - FUERZA
		auto it = particleForce.find(p);
		// ya está esta partícula
		if (it != particleForce.end()) {
			// los sets no pueden tener elementos duplicados
			it->second.insert(fg);
		}
		else {
			unordered_set<ForceGenerator*> fgSet;
			fgSet.insert(fg);
			particleForce.insert({ p, fgSet });
		}

		// SISTEMA FUERZA - PARTÍCULA
		auto it2 = forceParticle.find(fg);
		// ya está el generador de fuerzas
		if (it2 != forceParticle.end()) {
			it2->second.insert(p);
		}
		else {
			forceParticle.insert({ fg, {p} });
		}
	}

	void deleteParticleRegistry(Particle* particle) {
		for (auto it = forceParticle.begin(); it != forceParticle.end(); ++it) {
			it->second.erase(particle);
		}
		particleForce.erase(particle);
	}

	void deleteForceRegistry(ForceGenerator* fg) {
		for (auto it = particleForce.begin(); it != particleForce.end(); ++it) {
			it->second.erase(fg);
		}
		forceParticle.erase(fg);
	}

	void updateTime(double t) {
		// se aumenta el tiempo de todos los generadores
		for (auto it = forceParticle.begin(); it != forceParticle.end(); ++it) {
			it->first->updateTime(t);
		}

		// sacar del registro los generadores no vivos
		for (auto it = begin(forceParticle); it != end(forceParticle);) {
			if (!it->first->isAlive()) {
				// si se elimina a partir del iterador, entonces si se devuelve el iterador
				for (auto it2 = particleForce.begin(); it2 != particleForce.end(); ++it2) {
					it2->second.erase(it->first);
				}
				it = forceParticle.erase(it);
			}
			else {
				++it;
			}
		}
	}
};