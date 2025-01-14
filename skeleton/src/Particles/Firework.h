#pragma once
#include <list>
#include "Particle.h"
#include "../ListParticles.h"
#include "../Generators/FireworkGenerator.h"

using namespace std;

class Firework : public Particle {
private:
	void explode(ListParticles* particles);

public:
	Firework(Vector3 pos, Vector3 vel, float invMasa, double damping, float lifeTime,
		float vSimulada, float radius, Vector4 color = Vector4(1, 0, 0, 1), ParticleType type = Default);

	virtual ~Firework() {

	}

	virtual void onDeath(ListParticles* particles);
};