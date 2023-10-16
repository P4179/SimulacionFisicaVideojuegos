#pragma once
#include "Particle.h"
#include "ParticleGenerator.h"
#include <list>

using namespace std;

class ParticleSystem {
private:
	std::list<Particle*> particles;
	std::list<ParticleGenerator*> particle_generators;
	Vector3 gravity;
	// temporal
	Vector3 pos;
	Vector3 dir;

public:
	ParticleSystem(Vector3 pos, Vector3 dir, Vector3 gravity = Vector3(0, -10, 0)) :
		pos(pos), dir(dir), gravity(gravity) {}

	~ParticleSystem() {
		for (auto& particle : particles) {
			delete particle;
		}
		particles.clear();
		for (auto& gen : particle_generators) {
			delete gen;
		}
		particle_generators.clear();
	}

	void integrate(double t) {
		// eliminar partículas no vivas
		particles.erase(std::remove_if(particles.begin(), particles.end(),
			[](Particle* particle) {
				if (particle->isAlive()) {
					return false;
				}
				else {
					delete particle;
					return true;
				}
			}), particles.end());

		// update de cada partícula
		for (auto particle : particles) {
			particle->integrate(t);
		}

		// generar más partículas
		Particle* particle = new Particle(Vector3(0, 0, 0), Vector3(0, 1, 0), Vector3(0, -10, 0), 0.8, 200);
		particles.push_back(particle);
	}

	ParticleGenerator* getParticleGenerator(string name) const {

	}

	void generateFireworkSystem() {

	}
};