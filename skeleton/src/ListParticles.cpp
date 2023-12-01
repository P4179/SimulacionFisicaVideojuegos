#include "Particles/Particle.h"
#include "ListParticles.h"
#include "checkML.h"
#include <iostream>

void ListParticles::removeExcess() {
	if (listP.size() > maxParticles) {
		int num = listP.size() - maxParticles;
		for (int i = 0; i < num; ++i) {

			registry->deleteParticleRegistry(listP.front());

			delete listP.front();
			listP.pop_front();
		}
	}
}

ListParticles::ListParticles(int maxParticles, ParticleForceRegistry* registry) :
	maxParticles(maxParticles), registry(registry) {}

ListParticles::~ListParticles() {
	kill();
}

void ListParticles::add(list<Particle*> newListP) {
	for (auto particle : newListP) {
		listP.push_back(particle);
	}
}

void ListParticles::kill() {
	for (auto& particle : listP) {
		registry->deleteParticleRegistry(particle);
		delete particle;
	}
	listP.clear();
}

void ListParticles::refresh() {
	// eliminar part�culas no vivas
	listP.erase(std::remove_if(listP.begin(), listP.end(),
		[this](Particle* particle) {
			if (particle->isAlive()) {
				return false;
			}
			else {
				particle->onDeath(this);

				registry->deleteParticleRegistry(particle);

				delete particle;
				return true;
			}
		}), listP.end());

	// se hace despu�s del refresh y no justamente al a�adir las part�culas
	// porque si se trata de un firework se van a crear part�culas cuando este se muere
	// Entonces, puede que se exceda la cantidad m�xima, se eliminen part�culas
	// y se rompa la lista pues se est� recorriendo
	removeExcess();
}

void ListParticles::integrate(double t) {
	registry->updateForces(t);

	// update de cada part�cula
	for (auto particle : listP) {
		particle->integrate(t);
	}
}