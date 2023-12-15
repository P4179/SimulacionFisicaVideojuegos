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
	vector<std::pair<ForceGenerator<Particle>*, Particle*>> forcesParticles;

public:
	// el vector de fuerzas va a servir para tener un registro del resto de fuerzas que se están aplicando
	LauncherParticleGen(string name);

	// solo es un lanzador de particula
	// no actualiza ni genera nuevas particulas
	virtual list<Particle*> generateParticles() { return {}; }
	virtual void update() {}

	void launch(ListParticles* particles,
		const vector<std::pair<ForceGenerator<Particle>*, Particle*>>& forcesParticles,
		const unordered_set<ForceGenerator<Particle>*>& forceGens = unordered_set<ForceGenerator<Particle>*>());

	inline void addForce(ForceGenerator<Particle>* force, ListParticles* particles) override {
		if (!forcesParticles.empty()) {
			for (auto forceParticle : forcesParticles) {
				particles->add<vector<ForceGenerator<Particle>*>>({ forceParticle.second }, { force });
			}
			ForceParticleGenerator::addForce(force, particles);
		}
	}

	virtual inline void clear() {
		forcesParticles.clear();
		ForceParticleGenerator::clear();
	}

	vector<Particle*> findParticleByForce(ForceGenerator<Particle>* force);
};
