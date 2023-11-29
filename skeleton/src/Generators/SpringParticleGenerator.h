#pragma once
#include "ForceParticleGenerator.h"
#include "../ForceGenerators/ForceGenerator.h"

class SpringParticleGenerator : public ForceParticleGenerator {
private:
	vector<std::pair<ForceGenerator*, Particle*>> springs;

public:
	// el vector de fuerzas va a servir para tener un registro del resto de fuerzas que se están aplicando
	SpringParticleGenerator(string name, vector<std::pair<ForceGenerator*, Particle*>> springs,
		const vector<ForceGenerator*>& forceGens = vector<ForceGenerator*>()) :
		ForceParticleGenerator(name, Vector3(), Vector3(), ParticleInfo(), double(), int(), Vector3(), Vector3(), forceGens),
		springs(springs) {}

	virtual void update() {}

	virtual void init(ListParticles* particles) {
		// resto de fuerzas que queremos aplicar + fuerza muelle que tiene asociada esa particula
		vector<ForceGenerator*> fGens(forceGens.size() + 1);
		// se inserta el resto de fuerzas
		fGens.insert(fGens.begin(), forceGens.begin(), forceGens.end());
		for (auto spring : springs) {
			fGens[fGens.size() - 1] = spring.first;
			particles->add({ spring.second }, forceGens);
		}
	}

	inline void addForce(ForceGenerator* force, ListParticles* particles) override {
		for (auto spring : springs) {
			particles->registerForceParticle(spring.second, force);
		}
		ForceParticleGenerator::addForce(force, particles);
	}
};
