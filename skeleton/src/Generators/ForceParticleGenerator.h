#pragma once
#include "ParticleGenerator.h"
#include <unordered_set>

using namespace std;

template <typename T>
class ForceParticleGenerator : public ParticleGenerator<T> {
protected:
	unordered_set<ForceGenerator<Particle>*> forceGens;

	/*
	Particle* createParticle(Vector3 pos, Vector3 vel) override {
		return new Particle(pos, vel, _info.invMasa, _info.damping, _info.lifeTime, _info.vSimulada, _info.radius, _info.color);
	}
	*/

	ForceParticleGenerator(string name, Vector3 meanPos, Vector3 meanVel, ParticleInfo info, double genProbability, int numParticles, const unordered_set<ForceGenerator<T>*>& forceGens = unordered_set<ForceGenerator<T>*>()) :
		ParticleGenerator(name, meanPos, meanVel, info, genProbability, numParticles), forceGens(forceGens) {}
public:

	// solo para particulas render y fuerzas locales
	virtual void update(ListParticles* particles) {
		// no hace falta indicar el template, ya lo supone el compilador
		particles->add(generateParticles(), forceGens);
	}

	inline bool containsForce(ForceGenerator<Particle>* force) {
		return forceGens.find(force) != forceGens.end();
	}

	inline bool removeForce(ForceGenerator<Particle>* force) {
		auto it = forceGens.find(force);
		if (it != forceGens.end()) {
			forceGens.erase(it);
			return true;
		}
		return false;
	}

	// la lista de particulas no se utiliza
	// sin embargo, se puede utilizar en las clases hijas que solo hagan
	// un commit inicial de particulas y luego, se les quiere poner mas fuerzas
	virtual inline void addForce(ForceGenerator<Particle>* force, ListParticles* particles) {
		forceGens.insert(force);
	}

	virtual inline void clear() {
		forceGens.clear();
	}
};