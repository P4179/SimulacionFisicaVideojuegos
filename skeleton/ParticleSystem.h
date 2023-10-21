#pragma once
#include "Particle.h"
#include <list>
#include <unordered_map>
#include "UniformParticleGenerator.h"

using namespace std;

class ParticleSystem {
private:
	const float DAMPING = 0.8;

	std::list<Particle*> particles;
	std::list<ParticleGenerator*> particle_generators;
	unordered_map<string, ParticleGenerator*> particleGenFindByName;
	Vector3 gravity;

	template<typename T, typename ...Ts>
	void addParticleGenerator(string name, Vector3 pos, Vector3 vel, ParticleInfo info, double genProbability, int numParticles, Ts&& ...args) {
		ParticleGenerator* generator = new T(forward<Ts>(args)..., name, pos, vel, info, genProbability, numParticles);
		particle_generators.push_back(generator);
		particleGenFindByName.insert({ name, generator });
	}

public:
	ParticleSystem(Vector3 gravity = Vector3(0, -10, 0)) : gravity(gravity) {
		ParticleInfo info;
		info.ac = gravity;
		info.color = Vector4(0.337, 0.192, 0.8, 1);
		info.damping = DAMPING;
		info.lifeTime = 50;
		info.radius = 1.4;
		info.vSimulada = 50.0;

		addParticleGenerator<UniformParticleGenerator>("fuente", Vector3(0, 0, 0), Vector3(0, 1, 0), info, 0.3, 1, Vector3(0.2, 0, 0.2), Vector3(2, 0, 2));
		/*
		string name = "fuente";
		auto fountain = new UniformParticleGenerator(Vector3(0.2, 0, 0.2), Vector3(2, 0, 2), name, Vector3(0, 0, 0), Vector3(0, 1, 0), info, 0.3, 1);
		particle_generators.push_back(fountain);
		particleGenFindByName.insert({ name, fountain });
		*/

		info.ac = gravity;
		info.color = Vector4(0.18, 0.596, 0.839, 1);
		info.damping = DAMPING;
		info.lifeTime = 50;
		info.radius = 1;
		info.vSimulada = 40.0;
		addParticleGenerator<UniformParticleGenerator>("manguera", Vector3(-40, 10, 0), Vector3(0.6, 1, 0), info, 0.3, 1, Vector3(0, 0.3, 0.2), Vector3(1, 0, 1));

		info.ac = gravity;
		info.color = Vector4(0.361, 0.529, 0.729, 1);
		info.damping = DAMPING;
		info.lifeTime = 50;
		info.radius = 0.6;
		info.vSimulada = 35.0;
		addParticleGenerator<UniformParticleGenerator>("lluvia", Vector3(0, 65, 0), Vector3(0, -1, 0), info, 0.5, 3, Vector3(0, 0, 0), Vector3(20, 2, 20));
	}

	virtual ~ParticleSystem() {
		for (auto& particle : particles) {
			delete particle;
		}
		particles.clear();
		for (auto& gen : particle_generators) {
			delete gen;
		}
		particle_generators.clear();
	}

	void refresh() {
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
	}

	void integrate(double t) {
		refresh();

		// update de cada partícula
		for (auto particle : particles) {
			particle->integrate(t);
		}

		// generar más partículas
		// un generador de partículas de devuelve una lista de partículas
		// y se mete en la lista de partículas del sistema
		list<Particle*> particlesGenerated = getParticleGenerator("lluvia")->generateParticles();
		for (auto particle : particlesGenerated) {
			particles.push_back(particle);
		}
	}

	ParticleGenerator* getParticleGenerator(string name) const {
		auto it = particleGenFindByName.find(name);
		if (it == particleGenFindByName.end()) {
			throw new exception("It does not exit a particle generator with that name");
		}
		else {
			return (*it).second;
		}
	}

	void generateFireworkSystem() {

	}
};