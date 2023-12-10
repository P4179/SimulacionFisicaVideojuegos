#pragma once
#include "SpringForceGenerator.h"

class ElasticRubberForceGenerator : public SpringForceGenerator {
private:

public:
	ElasticRubberForceGenerator(string name, double k, double restingLength, Particle* other, float duration = -1.0);

	// en un muelle la fuerza solo se aplica cuando la distancia entre ambas supera la distancia de reposo
	virtual void updateForce(Particle* particle, double t);
};