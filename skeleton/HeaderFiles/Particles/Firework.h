#pragma once
#include "Particle.h"

class Firework : public Particle {
private:
	int type;

	std::list<Particle*> explode() {
		try {
			auto gen = FireworkGenerator::getGenerator(type);
			return gen->generateParticles();
		}
		catch (exception e) {
			cout << "0 particles were generated :" << e.what() << "\n";
		}
	}

public:
	Firework(int genType, Vector3 pos, Vector3 vel, Vector3 acReal, double damping, float lifeTime, float vSimulada, float radius, Vector4 color = Vector4(1, 0, 0, 1), ParticleType type = Default) :
		Particle(pos, vel, acReal, damping, lifeTime, vSimulada, radius, color, type), type(genType) {}

	virtual void onDeath(std::list<Particle*>& particles) {
		for (auto particle : explode()) {
			particles.push_back(particle);
		}
	}
};