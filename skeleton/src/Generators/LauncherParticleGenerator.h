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
		const vector<ForceGenerator*>& forceGens = vector<ForceGenerator*>()) {

		// se asignan los nuevos valores del lanzador
		this->forcesParticles = forcesParticles;
		this->forceGens = forceGens;

		// se lanza
		// resto de fuerzas que queremos aplicar + fuerza que tiene asociada esa particula
		vector<ForceGenerator*> fGens(forceGens.size() + 1);
		// se inserta el resto de fuerzas
		for (int i = 0; i < forceGens.size(); ++i) {
			fGens[i] = forceGens[i];
		}
		for (auto forceParticle : this->forcesParticles) {
			fGens[fGens.size() - 1] = forceParticle.first;
			particles->add({ forceParticle.second }, fGens);
		}
	}

	inline void addForce(ForceGenerator* force, ListParticles* particles) override {
		if (!forcesParticles.empty()) {
			for (auto forceParticle : forcesParticles) {
				particles->registerForceParticle(forceParticle.second, force);
			}
			ForceParticleGenerator::addForce(force, particles);
		}
	}

	inline void clear() {
		forcesParticles.clear();
		forceGens.clear();
	}
};
