#pragma once
#include <iostream>
#include "ForceParticleGenerator.h"
#include "../ListParticles.h"

using namespace std;

class GaussianParticleGenerator : public ForceParticleGenerator {
protected:
	Vector3 std_dev_pos;
	Vector3 std_dev_vel;

	double variation(double standardDesviation) {
		// distribución normal (media, desviación estándar)
		// la dist normal es un tipo de distribución en la que la gran mayoría de datos tienden hacia la mitad del rango
		// media -> indica donde está colocado esa mitad del rango
		// desviación estándar -> indica como de distribuidos está un conjuntos de datos respecto de la media
		// de modo que cuanto menor sea, habrá más datos cerca de la media

		// si la desviación estándar es 0 se produce un error
		if (standardDesviation > 0) {
			normal_distribution<double> dist = normal_distribution<double>(0, standardDesviation);
			return dist(_mt);
		}
		else {
			return 0;
		}
	}

public:
	GaussianParticleGenerator(string name, Vector3 mean_pos, Vector3 mean_vel, ParticleInfo info, double generation_probability, int num_particles, Vector3 stdDevVel, Vector3 stdDevPos, const unordered_set<ForceGenerator<Particle>*>& forceGens = {}) :
		ForceParticleGenerator(name, mean_pos, mean_vel, info, generation_probability, num_particles, forceGens),
		std_dev_pos(stdDevPos), std_dev_vel(stdDevVel) {}

	list<Particle*> generateParticles() {
		list<Particle*> particles;

		if (probability()) {
			for (int i = 0; i < _num_particles; ++i) {
				Vector3 vel = _mean_vel;
				vel.x += variation(std_dev_vel.x);
				vel.y += variation(std_dev_vel.y);
				vel.z += variation(std_dev_vel.z);

				Vector3 pos = _mean_pos;
				pos.x += variation(std_dev_pos.x);
				pos.y += variation(std_dev_pos.y);
				pos.z += variation(std_dev_pos.z);

				particles.push_back(createParticle(pos, vel));
			}
		}

		return particles;
	}


	inline void setStdDevVel(Vector3 newVel) {
		this->std_dev_vel = newVel;
	}

	inline Vector3 getStdDevVel() const {
		return this->std_dev_vel;
	}
};