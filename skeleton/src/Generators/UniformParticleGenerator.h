#pragma once
#include "ForceParticleGenerator.h"
#include "../ListParticles.h"

using namespace std;

class UniformParticleGenerator : public ForceParticleGenerator {
private:
	Vector3 vel_width;
	Vector3 pos_width;
	uniform_real_distribution<double> dist;

public:
	UniformParticleGenerator(string name, Vector3 mean_pos, Vector3 mean_vel, ParticleInfo info, double generation_probability,
		int num_particles, Vector3 velWidth, Vector3 posWidth, const unordered_set<ForceGenerator<Particle>*>& forceGens = {},
		PxPhysics* gPhysics = nullptr, PxScene* gScene = nullptr);

	virtual list<Particle*> generateParticles();
};