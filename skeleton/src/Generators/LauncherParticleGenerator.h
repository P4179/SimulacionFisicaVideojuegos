#pragma once
#include "ForceParticleGenerator.h"
#include "../ForceGenerators/ForceGenerator.h"

/*
Sirve para realizar el lanzamiento inicial de una serie de particulas, cada una con una fuerza especifica
Ademas, se puede indicar un conjunto de fuerzas comun que afectan a todas las particulas
*/
class LauncherParticleGen : public ForceParticleGenerator {
private:
	// sirve para indicar la fuerza que le afecta a cada particula
	vector<std::pair<ForceGenerator*, Particle*>> forcesParticles;

public:
	// el vector de fuerzas va a servir para tener un registro del resto de fuerzas que se están aplicando
	LauncherParticleGen(string name) : ForceParticleGenerator(name, Vector3(), Vector3(), ParticleInfo(), double(), int(), Vector3(), Vector3()),
		forcesParticles() {}

	virtual void update() {}

	void launch(ListParticles* particles,
		const vector<std::pair<ForceGenerator*, Particle*>>& forcesParticles,
		const unordered_set<ForceGenerator*>& forceGens = unordered_set<ForceGenerator*>()) {

		// se asignan los nuevos valores del lanzador
		this->forcesParticles = forcesParticles;
		this->forceGens = forceGens;

		// resto de fuerzas que queremos aplicar + fuerza que tiene asociada esa particula
		vector<ForceGenerator*> currentForces(forceGens.size() + 1);
		// se inserta el resto de fuerzas
		// copy los elementos de un contenedor de origen a uno de destino (tiene que haber espacio suficiente)
		std::copy(forceGens.begin(), forceGens.end(), currentForces.begin());
		for (auto forceParticle : this->forcesParticles) {
			// se inserta la fuerza de muelle que le corresponde a esa particula
			currentForces[currentForces.size() - 1] = forceParticle.first;
			particles->add({ forceParticle.second }, currentForces);
		}
	}

	inline void addForce(ForceGenerator* force, ListParticles* particles) override {
		if (!forcesParticles.empty()) {
			for (auto forceParticle : forcesParticles) {
				particles->add<vector<ForceGenerator*>>({ forceParticle.second }, { force });
			}
			ForceParticleGenerator::addForce(force, particles);
		}
	}

	virtual inline void clear() {
		forcesParticles.clear();
		ForceParticleGenerator::clear();
	}

	vector<Particle*> findParticleByForce(ForceGenerator* force) {
		vector<Particle*> particles;

		// fuerza comun, se devuelven todas las particulas
		if (forceGens.find(force) != forceGens.end()) {
			for (auto forceParticle : forcesParticles) {
				particles.push_back(forceParticle.second);
			}
		}
		else {
			for (auto forceParticle : forcesParticles) {
				if (forceParticle.first == force) {
					particles.push_back(forceParticle.second);
				}
			}
		}
		return particles;
	}
};
