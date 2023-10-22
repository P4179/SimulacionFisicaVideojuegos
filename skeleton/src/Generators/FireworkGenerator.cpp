#include "FireworkGenerator.h"
#include "../Particles/Firework.h"
#include "../checkML.h"

// importante inicializar el vector, sino se produce error
vector<FireworkGenerator*> FireworkGenerator::fireworks = vector<FireworkGenerator*>(0, nullptr);

Particle* FireworkGenerator::createParticle(Vector3 velVariation, Vector3 posVariation) {
	// número entre 0 y fireworks.size() - 1 con la misma probabilidad
	// solo se genera números enteros
	uniform_int_distribution<int> dist = uniform_int_distribution<int>(0, fireworks.size() - 1);
	int rnd = dist(_mt);

	try {
		Firework* fire = new Firework(getGenerator(rnd), posVariation, velVariation, _info.ac, _info.damping, _info.lifeTime, _info.vSimulada, _info.radius, _info.color);
		return fire;
	}
	// se propaga la excepción
	catch (exception e) {
		throw exception(e.what());
	}
}

FireworkGenerator::FireworkGenerator(string name, Vector3 mean_pos, Vector3 mean_vel, ParticleInfo info, int num_particles, Vector3 stdDevVel, Vector3 stdDevPos) :
	GaussianParticleGenerator(name, mean_pos, mean_vel, info, 1.0, num_particles, stdDevVel, stdDevPos) {}