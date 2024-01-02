#pragma once
#include "FireworkGenerator.h"
#define _USE_MATH_DEFINES
#include <math.h>

class CircleGenerator : public FireworkGenerator {
private:
	// tienen que ser vectores unitarios
	Vector3 dirSin, dirCos;

public:
	CircleGenerator(string name, Vector3 mean_pos, ParticleInfo info, int num_particles, Vector3 stdDevVel, Vector3 dirSin, Vector3 dirCos, const unordered_set<ForceGenerator<Particle>*>& forceGens = unordered_set<ForceGenerator<Particle>*>());

	list<Particle*> generateParticles();

	inline void setDirs(const Vector3& newDirSin, const Vector3& newDirCos) {
		this->dirSin = newDirSin;
		this->dirCos = newDirCos;

		// dos vectores son perpendiculares si el ángulo que forman es 90º
		// el producto escalar de dos vectores es a·b = |a||b|cos(X)
		// y el cos(90º) es 0, por lo tanto, su producto escalar es 0
		// de forma que si el producto escalar de dos vectores es 0, quiere decir que son perpendiculares
		int escalar = dirSin.x * dirCos.x + dirSin.y * dirCos.y + dirSin.z * dirCos.z;
		if (escalar != 0) {
			throw exception("Vectors are perpendicular");
		}
	}
};