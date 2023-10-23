#pragma once
#include <iostream>
#include "ParticleGenerator.h"
#include "../ListParticles.h"

using namespace std;

class GaussianParticleGenerator : public ParticleGenerator {
protected:
	Vector3 std_dev_pos;
	Vector3 std_dev_vel;

	double variation(double standardDesviation);

	virtual Particle* createParticle(Vector3 velVariation, Vector3 posVariation);

public:
	GaussianParticleGenerator(string name, Vector3 mean_pos, Vector3 mean_vel, ParticleInfo info, double generation_probability, int num_particles, Vector3 stdDevVel, Vector3 stdDevPos);

	list<Particle*> generateParticles();

	void update(ListParticles* particles);

	inline void setStdDevVel(Vector3 newVel) {
		this->std_dev_vel = newVel;
	}

	inline Vector3 getStdDevVel() const {
		return this->std_dev_vel;
	}
};