#include "CircleGenerator.h"
#include "../checkML.h"

CircleGenerator::CircleGenerator(string name, Vector3 mean_pos, ParticleInfo info, int num_particles, Vector3 stdDevVel, Vector3 dirSin, Vector3 dirCos) :
	FireworkGenerator(name, mean_pos, Vector3(0, 0, 0), info, num_particles, stdDevVel, Vector3(0, 0, 0)), dirSin(dirSin), dirCos(dirCos) {
	// dos vectores son perpendiculares si el ángulo que forman es 90º
	// el producto escalar de dos vectores es a·b = |a||b|cos(X)
	// y el cos(90º) es 0, por lo tanto, su producto escalar es 0
	// de forma que si el producto escalar de dos vectores es 0, quiere decir que son perpendiculares
	int escalar = dirSin.x * dirCos.x + dirSin.y * dirCos.y + dirSin.z * dirCos.z;
	if (escalar != 0) {
		throw exception("Vectors are perpendicular");
	}
}

list<Particle*> CircleGenerator::generateParticles() {
	list<Particle*> particles;

	double probability = _u(_mt);
	if (_generation_probability > probability) {

		int sectionDegrees = 360 / _num_particles;
		for (int i = 0; i < _num_particles; ++i) {
			double sectionRadians = (sectionDegrees * i) * (_Pi / 180);
			Vector3 vel = dirSin * sin(sectionRadians) + dirCos * cos(sectionRadians);
			vel.x += variation(std_dev_vel.x);
			vel.y += variation(std_dev_vel.y);
			vel.z += variation(std_dev_vel.z);

			try {
				particles.push_back(createParticle(vel, _mean_pos));
			}
			catch (exception e) {
				cout << "0 particles were created: " << e.what() << "\n";
			}
		}
	}

	return particles;
}