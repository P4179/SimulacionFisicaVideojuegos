#pragma once
#include <list>
#include "ParticleForceRegistry.h"

class Particle;
class ForceGenerator;

using namespace std;

class ListParticles {
private:
	int maxParticles;
	std::list<Particle*> listP;
	ParticleForceRegistry<Particle, ForceGenerator>* registry;

	void removeExcess();

public:
	ListParticles(int maxParticles, ParticleForceRegistry<Particle, ForceGenerator>* registry);

	virtual ~ListParticles();

	template<typename T>
	void add(list<Particle*> newListP, const T& forceGens) {
		for (auto particle : newListP) {
			// si la particula no se ha encontrado se mete
			// sino, solo se le ponen las nuevas fuerzas
			if (std::find(listP.begin(), listP.end(), particle) == listP.end()) {
				listP.push_back(particle);
			}

			for (auto forceGen : forceGens) {
				registry->addRegistry(forceGen, particle);
			}
		}
	}

	void add(list<Particle*> newListP);

	void kill();

	void refresh();

	void integrate(double t);
};