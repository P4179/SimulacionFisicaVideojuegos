#include "FireworkGenerator.h"
#include "../Particles/Firework.h"
#include "../checkML.h"

// importante inicializar el vector, sino se produce error
vector<FireworkGenerator*> FireworkGenerator::fireworks = vector<FireworkGenerator*>(0, nullptr);

Particle* FireworkGenerator::createParticle(Vector3 vel, Vector3 pos) {
	return new Firework(pos, vel, _info.ac, _info.damping, _info.lifeTime, _info.vSimulada, _info.radius, _info.color);
}

FireworkGenerator::FireworkGenerator(string name, Vector3 mean_pos, Vector3 mean_vel, ParticleInfo info, int num_particles, Vector3 stdDevVel, Vector3 stdDevPos) :
	GaussianParticleGenerator(name, mean_pos, mean_vel, info, 1.0, num_particles, stdDevVel, stdDevPos) {}