#pragma once
#include <list>
#include "Particle.h"
#include "../ListParticles.h"
#include "../Generators/FireworkGenerator.h"

using namespace std;

class Firework : public Particle {
private:
	FireworkGenerator* gen;

	list<Particle*> explode() {
		gen->setOrigin(pose.p);
		gen->setProbability(0.9);
		return gen->generateParticles();
	}

public:
	Firework(FireworkGenerator* gen, Vector3 pos, Vector3 vel, Vector3 acReal, double damping, float lifeTime,
		float vSimulada, float radius, Vector4 color = Vector4(1, 0, 0, 1), ParticleType type = Default) :
		Particle(pos, vel, acReal, damping, lifeTime, vSimulada, radius, color, type), gen(gen) {}

	virtual void onDeath(ListParticles* particles) {
		particles->add(explode());
		/*
		for (auto particle : explode()) {
			particles.push_back(particle);
		}
		*/
	}
};