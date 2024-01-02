#include "UniformParticleGenerator.h"

UniformParticleGenerator::UniformParticleGenerator(string name, Vector3 mean_pos, Vector3 mean_vel, ParticleInfo info, double generation_probability,
	int num_particles, Vector3 velWidth, Vector3 posWidth, const unordered_set<ForceGenerator<Particle>*>& forceGens, PxPhysics* gPhysics, PxScene* gScene) :
	ForceParticleGenerator(name, mean_pos, mean_vel, info, generation_probability, num_particles, forceGens, gPhysics, gScene),
	vel_width(velWidth), pos_width(posWidth), dist(uniform_real_distribution<double>(-1, 1)) {}

list<Particle*> UniformParticleGenerator::generateParticles() {
	list<Particle*> particles;

	if (probability()) {
		for (int i = 0; i < _num_particles; ++i) {
			Vector3 vel = _mean_vel;
			vel.x += dist(_mt) * vel_width.x;
			vel.y += dist(_mt) * vel_width.y;
			vel.z += dist(_mt) * vel_width.z;

			Vector3 pos = _mean_pos;
			pos.x += dist(_mt) * pos_width.x;
			pos.y += dist(_mt) * pos_width.y;
			pos.z += dist(_mt) * pos_width.z;

			Particle* particle = createParticle(pos, vel);
			particles.push_back(particle);
		}
	}

	return particles;
}