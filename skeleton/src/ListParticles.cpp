#include "Particles/Particle.h"
#include "ListParticles.h"
#include "checkML.h"
#include <iostream>

ListParticles::ListParticles(int maxParticles) : maxParticles(maxParticles) {}

ListParticles::~ListParticles() {
	kill();
}

void ListParticles::add(list<Particle*> newListP) {
	if (listP.size() < maxParticles) {
		for (auto particle : newListP) {
			listP.push_back(particle);
		}
	}
	else {
		auto it = listP.begin();
		for (int i = 0; i < newListP.size(); ++i) {
			(*it)->setAlive(false);
			++it;
		}

		for (auto it2 = newListP.begin(); it2 != newListP.end(); ++it2) {
			listP.push_back(*it2);
		}
	}
}

void ListParticles::kill() {
	for (auto& particle : listP) {
		delete particle;
	}
	listP.clear();
}

void ListParticles::refresh() {
	// eliminar partículas no vivas
	listP.erase(std::remove_if(listP.begin(), listP.end(),
		[this](Particle* particle) {
			if (particle->isAlive()) {
				return false;
			}
			else {
				particle->onDeath(this);
				delete particle;
				return true;
			}
		}), listP.end());
	//std::cout << listP.size() << "\n";
}

void ListParticles::integrate(double t) {
	// update de cada partícula
	for (auto particle : listP) {
		particle->integrate(t);
	}
}