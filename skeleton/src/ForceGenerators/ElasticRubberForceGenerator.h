#pragma once
#include "SpringForceGenerator.h"

class ElasticRubberForceGenerator : public SpringForceGenerator {
private:

public:
	ElasticRubberForceGenerator(string name, double k, double restingLength, Particle* other, float duration = -1.0) :
		SpringForceGenerator(name, k, restingLength, other, duration) {}

	// en un muelle la fuerza solo se aplica cuando la distancia entre ambas supera la distancia de reposo
	virtual void updateForce(Particle* particle, double t) {
		Vector3 relativePos = this->getPosOtherEnd() - particle->getPos();
		if (relativePos.magnitude() > restingLength) {
			SpringForceGenerator::updateForce(particle, t);
		}
	}
};