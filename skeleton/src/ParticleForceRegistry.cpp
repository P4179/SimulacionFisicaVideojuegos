/*
#include "ParticleForceRegistry.h"

ParticleForceRegistry::ParticleForceRegistry() : particleForce(), forceParticle() {}

void ParticleForceRegistry::updateForces(double t) {
	for (auto it = forceParticle.begin(); it != forceParticle.end(); ++it) {
		auto particles = it->second;
		for (auto particle : particles) {
			it->first->updateForce(particle, t);
		}
	}
}

void ParticleForceRegistry::addRegistry(ForceGenerator* fg, Particle* p) {
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

void ParticleForceRegistry::deleteParticleRegistry(Particle* particle) {
	for (auto it = forceParticle.begin(); it != forceParticle.end(); ++it) {
		it->second.erase(particle);
	}
	particleForce.erase(particle);
}

void ParticleForceRegistry::deleteForceRegistry(ForceGenerator* fg) {
	for (auto it = particleForce.begin(); it != particleForce.end(); ++it) {
		it->second.erase(fg);
	}
	forceParticle.erase(fg);
}

void ParticleForceRegistry::updateTime(double t) {
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
*/