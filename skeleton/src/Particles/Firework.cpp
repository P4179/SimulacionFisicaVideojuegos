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

Firework::Firework(Vector3 pos, Vector3 vel, float invMasa, double damping, float lifeTime,
	float vSimulada, float radius, Vector4 color, ParticleType type) :
	Particle(pos, vel, invMasa, damping, lifeTime, vSimulada, radius, color) {}

void Firework::onDeath(ListParticles* particles) {
	particles->add(explode());
}