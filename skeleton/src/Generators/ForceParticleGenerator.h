#pragma once
#include "ParticleGenerator.h"
#include <unordered_set>

using namespace std;

class ForceParticleGenerator : public ParticleGenerator {
protected:
	unordered_set<ForceGenerator<Particle>*> forceGens;

	ForceParticleGenerator(string name, Vector3 meanPos, Vector3 meanVel, ParticleInfo info, double genProbability, int numParticles, const unordered_set<ForceGenerator<Particle>*>& forceGens = unordered_set<ForceGenerator<Particle>*>()) :
		ParticleGenerator(name, meanPos, meanVel, info, genProbability, numParticles), forceGens(forceGens) {}
public:

	// para fuerzas locales
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