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
};