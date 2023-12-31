#include "LauncherParticleGenerator.h"
#include "../checkML.h"

LauncherParticleGen::LauncherParticleGen(string name) : ForceParticleGenerator(name, Vector3(), Vector3(), ParticleInfo(), double(), int()),
forcesParticles() {}

void LauncherParticleGen::launch(ListParticles* particles,
	const vector<std::pair<ForceGenerator<Particle>*, Particle*>>& forcesParticles,
	const unordered_set<ForceGenerator<Particle>*>& forceGens) {

	// se asignan los nuevos valores del lanzador
	this->forcesParticles = forcesParticles;
	this->forceGens = forceGens;

	// resto de fuerzas que queremos aplicar + fuerza que tiene asociada esa particula
	vector<ForceGenerator<Particle>*> currentForces(forceGens.size() + 1);
	// se inserta el resto de fuerzas
	// copy los elementos de un contenedor de origen a uno de destino (tiene que haber espacio suficiente)
	std::copy(forceGens.begin(), forceGens.end(), currentForces.begin());
	for (auto forceParticle : this->forcesParticles) {
		// se inserta la fuerza de muelle que le corresponde a esa particula
		currentForces[currentForces.size() - 1] = forceParticle.first;
		particles->add({ forceParticle.second }, currentForces);
	}
}

vector<Particle*> LauncherParticleGen::findParticleByForce(ForceGenerator<Particle>* force) {
	vector<Particle*> particles;

	// fuerza comun, se devuelven todas las particulas
	if (forceGens.find(force) != forceGens.end()) {
		for (auto forceParticle : forcesParticles) {
			particles.push_back(forceParticle.second);
		}
	}
	else {
		for (auto forceParticle : forcesParticles) {
			if (forceParticle.first == force) {
				particles.push_back(forceParticle.second);
			}
		}
	}
	return particles;
}