#include "GaussianParticleGenerator.h"

double GaussianParticleGenerator::variation(double standardDesviation) {
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

Particle* GaussianParticleGenerator::createParticle(Vector3 vel, Vector3 pos) {
	return new Particle(pos, vel, _info.ac, _info.damping, _info.lifeTime, _info.vSimulada, _info.radius, _info.color);
}

GaussianParticleGenerator::GaussianParticleGenerator(string name, Vector3 mean_pos, Vector3 mean_vel, ParticleInfo info, double generation_probability, int num_particles, Vector3 stdDevVel, Vector3 stdDevPos) :
	ParticleGenerator(name, mean_pos, mean_vel, info, generation_probability, num_particles),
	std_dev_pos(stdDevPos), std_dev_vel(stdDevVel) {}

list<Particle*> GaussianParticleGenerator::generateParticles() {
	list<Particle*> particles;

	double probability = _u(_mt);
	if (_generation_probability > probability) {

		for (int i = 0; i < _num_particles; ++i) {
			Vector3 vel = _mean_vel;
			vel.x += variation(std_dev_vel.x);
			vel.y += variation(std_dev_vel.y);
			vel.z += variation(std_dev_vel.z);

			Vector3 pos = _mean_pos;
			pos.x += variation(std_dev_pos.x);
			pos.y += variation(std_dev_pos.y);
			pos.z += variation(std_dev_pos.z);

			particles.push_back(createParticle(vel, pos));
		}
	}

	return particles;
}

void GaussianParticleGenerator::update(ListParticles* particles) {
	particles->add(generateParticles());
}