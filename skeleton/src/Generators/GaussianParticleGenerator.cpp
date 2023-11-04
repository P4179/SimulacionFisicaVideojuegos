#include "GaussianParticleGenerator.h"

double GaussianParticleGenerator::variation(double standardDesviation) {
	// distribuci�n normal (media, desviaci�n est�ndar)
	// la dist normal es un tipo de distribuci�n en la que la gran mayor�a de datos tienden hacia la mitad del rango
	// media -> indica donde est� colocado esa mitad del rango
	// desviaci�n est�ndar -> indica como de distribuidos est� un conjuntos de datos respecto de la media
	// de modo que cuanto menor sea, habr� m�s datos cerca de la media

	// si la desviaci�n est�ndar es 0 se produce un error
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