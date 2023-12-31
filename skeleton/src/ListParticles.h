#pragma once
#include <list>
#include "ParticleForceRegistry.h"

using namespace std;

class ListParticles {
private:
	int maxParticles;
	std::list<Particle*> listP;
	ParticleForceRegistry<Particle>* registry;

	void removeExcess();

public:
	ListParticles(int maxParticles, ParticleForceRegistry<Particle>* registry);

	virtual ~ListParticles();

	template<typename T>
	void add(list<Particle*> newListP, const T& forceGens) {
		for (auto& particle : newListP) {
			// si la particula no se ha encontrado se mete
			// sino, solo se le ponen las nuevas fuerzas
			auto it = std::find(listP.begin(), listP.end(), particle);
			if (it == listP.end()) {
				listP.push_back(particle);
			}

			for (auto forceGen : forceGens) {
				registry->addRegistry(forceGen, particle);
			}
		}
	}

	// YA NO SE USA, PORQUE ERA PARA METER PARTICULAS A LAS QUE NO LE AFECTARAN LAS FUERZAS
	void add(list<Particle*> newListP);

	void kill();

	void refresh();

	void integrate(double t);
};