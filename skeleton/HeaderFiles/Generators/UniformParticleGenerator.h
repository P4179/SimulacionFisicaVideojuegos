#pragma once
#include "ParticleGenerator.h"
#include "../../RenderUtils.hpp"

using namespace std;

class UniformParticleGenerator : public ParticleGenerator {
private:
	Vector3 vel_width;
	Vector3 pos_width;
	uniform_real_distribution<double> dist;

	double variation(double from, double to) {
		uniform_real_distribution<double> dist = uniform_real_distribution<double>(from, to);
		return dist(_mt);
	}

public:
	UniformParticleGenerator(Vector3 velWidth, Vector3 posWidth, string name, Vector3 mean_pos, Vector3 mean_vel, ParticleInfo info, double generation_probability = 1.0f, int num_particles = 3) :
		ParticleGenerator(name, mean_pos, mean_vel, info, generation_probability, num_particles),
		vel_width(velWidth), pos_width(posWidth), dist(uniform_real_distribution<double>(-1, 1)) {}

	virtual list<Particle*> generateParticles() {
		list<Particle*> particles;

		double probability = _u(_mt);
		// se generan partículas
		if (_generation_probability > probability) {

			for (int i = 0; i < _num_particles; ++i) {
				Vector3 vel = _mean_vel;
				vel.x += dist(_mt) * vel_width.x;
				vel.y += dist(_mt) * vel_width.y;
				vel.z += dist(_mt) * vel_width.z;

				Vector3 pos = _mean_pos;
				pos.x += dist(_mt) * pos_width.x;
				pos.y += dist(_mt) * pos_width.y;
				pos.z += dist(_mt) * pos_width.z;

				Particle* particle = new Particle(pos, vel, _info.ac, _info.damping, _info.lifeTime, _info.vSimulada, _info.radius, _info.color);
				particles.push_back(particle);
			}
		}

		return particles;
	}

	virtual void update(list<Particle*>& particles) {
		for (auto particle : generateParticles()) {
			particles.push_back(particle);
		}
	}
};