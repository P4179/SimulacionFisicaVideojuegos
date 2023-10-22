#pragma once
#include "GaussianParticleGenerator.h"
#include "../Particles/Firework.h"

class FireworkGenerator : public GaussianParticleGenerator {
private:
	static vector<FireworkGenerator*> fireworks;

	virtual Particle* createParticle(Vector3 velVariation, Vector3 posVariation) {
		// número entre 0 y fireworks.size() - 1 con la misma probabilidad
		// solo se genera números enteros
		uniform_int_distribution<int> dist = uniform_int_distribution<int>(0, fireworks.size() - 1);
		int rnd = dist(_mt);

		return new Firework(rnd, posVariation, velVariation, _info.ac, _info.damping, _info.lifeTime, _info.vSimulada, _info.radius, _info.color);
	}

public:
	// como la probabilidad es 1 siempre se generar partículas
	FireworkGenerator(Vector3 stdDevVel, Vector3 stdDevPos, string name, Vector3 mean_pos, Vector3 mean_vel, ParticleInfo info, int num_particles = 3) :
		GaussianParticleGenerator(stdDevVel, stdDevPos, name, mean_pos, mean_vel, info, 1.0, num_particles) {}

	// el fireworkGenerator contiene los generadores que se pueden utilizar para crear más partículas
	// cuando las partícula que ha creado mueran
	static void addFirework(FireworkGenerator* firework) {
		fireworks.push_back(firework);
	}

	static FireworkGenerator* getGenerator(int type) {
		if (type >= fireworks.size()) {
			throw exception("There is not any generator associated with that index");
		}
		return fireworks[type];
	}

	// no sucede nada cuando se produce el update
	virtual void update(list<Particle*>& particles) {}
};