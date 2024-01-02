#include "CircleGenerator.h"
#include "../checkML.h"

CircleGenerator::CircleGenerator(string name, Vector3 mean_pos, ParticleInfo info, int num_particles, Vector3 stdDevVel, Vector3 dirSin, Vector3 dirCos, const unordered_set<ForceGenerator<Particle>*>& forceGens) :
	FireworkGenerator(name, mean_pos, Vector3(0, 0, 0), info, num_particles, stdDevVel, Vector3(0, 0, 0), forceGens), dirSin(dirSin), dirCos(dirCos) {
	
	setDirs(dirSin, dirCos);
}

list<Particle*> CircleGenerator::generateParticles() {
	list<Particle*> particles;

	double probability = _u(_mt);
	if (_generation_probability > probability) {

		int sectionDegrees = 360 / _num_particles;
		for (int i = 0; i < _num_particles; ++i) {
			double sectionRadians = (sectionDegrees * i) * (M_PI / 180);
			Vector3 vel = dirSin * sin(sectionRadians) + dirCos * cos(sectionRadians);
			vel.x += variation(std_dev_vel.x);
			vel.y += variation(std_dev_vel.y);
			vel.z += variation(std_dev_vel.z);

			particles.push_back(createParticle(_mean_pos, vel));
		}
	}

	return particles;
}