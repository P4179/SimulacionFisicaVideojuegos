#pragma once
#include <iostream>
#include "ForceParticleGenerator.h"
#include "../ListParticles.h"

using namespace std;

class GaussianParticleGenerator : public ForceParticleGenerator {
protected:
	Vector3 std_dev_pos;
	Vector3 std_dev_vel;

	double variation(double standardDesviation);

public:
	GaussianParticleGenerator(string name, Vector3 mean_pos, Vector3 mean_vel, ParticleInfo info, double generation_probability,
		int num_particles, Vector3 stdDevVel, Vector3 stdDevPos, const unordered_set<ForceGenerator<Particle>*>& forceGens = {},
		PxPhysics* gPhysics = nullptr, PxScene* gScene = nullptr);

	list<Particle*> generateParticles();

	inline void setStdDevVel(Vector3 newVel) {
		this->std_dev_vel = newVel;
	}

	inline Vector3 getStdDevVel() const {
		return this->std_dev_vel;
	}
};