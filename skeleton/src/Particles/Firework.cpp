#include "Firework.h"

list<Particle*> Firework::explode() {
	try {
		FireworkGenerator* gen = FireworkGenerator::getRndGenerator();
		gen->setOrigin(pose.p);
		// las explosiones tienen un 0.9 de probabilidad de que ocurran
		gen->setProbability(0.9);
		return gen->generateParticles();
	}
	catch (exception e) {
		cout << "Firework could not explode: " << e.what() << "\n";
	}
}

Firework::Firework(Vector3 pos, Vector3 vel, Vector3 acReal, double damping, float lifeTime,
	float vSimulada, float radius, Vector4 color, ParticleType type) :
	Particle(pos, vel, acReal, damping, lifeTime, vSimulada, radius, color, type) {}

void Firework::onDeath(ListParticles* particles) {
	particles->add(explode());
}