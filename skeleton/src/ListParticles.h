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
	ParticleForceRegistry* registry;

	void removeExcess();

public:
	ListParticles(int maxParticles, ParticleForceRegistry* registry);

	virtual ~ListParticles();

	template<typename T>
	void add(list<Particle*> newListP, const T& forceGens) {
		for (auto particle : newListP) {
			listP.push_back(particle);

			for (auto& forceGen : forceGens) {
				registry->addRegistry(forceGen, particle);
			}

			/*
			for (int i = 0; i < forceGens.size(); ++i) {
				registry->addRegistry(forceGens[i], particle);
			}
			*/
		}
	}

	void add(list<Particle*> newListP);

	void registerForceParticle(Particle* particle, ForceGenerator* fg);

	void kill();

	void refresh();

	void integrate(double t);
};