#include "FireworkGenerator.h"
#include "../Particles/Firework.h"
#include "../checkML.h"

// importante inicializar el vector, sino se produce error
vector<FireworkGenerator*> FireworkGenerator::fireworks = vector<FireworkGenerator*>(0, nullptr);

const float FireworkGenerator::EXPLOSION_PROBABILITY = 0.8f;

Particle* FireworkGenerator::createParticle(Vector3 pos, Vector3 vel) {
	return new Firework(pos, vel, _info.renderParticle_data.invMasa, _info.damping, _info.lifeTime, _info.renderParticle_data.vSimulada, _info.sphere_data.radius, _info.color);
}

FireworkGenerator::FireworkGenerator(string name, Vector3 mean_pos, Vector3 mean_vel, ParticleInfo info, int num_particles, Vector3 stdDevVel, Vector3 stdDevPos, const unordered_set<ForceGenerator<Particle>*>& forceGens) :
	GaussianParticleGenerator(name, mean_pos, mean_vel, info, 1.0, num_particles, stdDevVel, stdDevPos, forceGens) {}