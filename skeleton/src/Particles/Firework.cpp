#include "Firework.h"
#include "../checkML.h"

void Firework::explode(ListParticles* particles) {
	try {
		FireworkGenerator* gen = FireworkGenerator::getRndGenerator();
		gen->setOrigin(pose.p);
		gen->init(particles);
	}
	catch (exception e) {
		cout << "Firework could not explode: " << e.what() << "\n";
	}
}

Firework::Firework(Vector3 pos, Vector3 vel, float invMasa, double damping, float lifeTime,
	float vSimulada, float radius, Vector4 color, ParticleType type) :
	Particle(pos, vel, invMasa, damping, lifeTime, vSimulada, radius, color) {}

void Firework::onDeath(ListParticles* particles) {
	explode(particles);
}