#include "ElasticRubberForceGenerator.h"

ElasticRubberForceGenerator::ElasticRubberForceGenerator(string name, double k, double restingLength, Particle* other, float duration) :
	SpringForceGenerator(name, k, restingLength, other, duration) {}

// en un muelle la fuerza solo se aplica cuando la distancia entre ambas supera la distancia de reposo
void ElasticRubberForceGenerator::updateForce(Particle* particle, double t) {
	Vector3 relativePos = this->getPosOtherEnd() - particle->getPos();
	if (relativePos.magnitude() > restingLength) {
		SpringForceGenerator::updateForce(particle, t);
	}
}