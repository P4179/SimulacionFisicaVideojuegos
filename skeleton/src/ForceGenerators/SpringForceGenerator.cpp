#include "SpringForceGenerator.h"

SpringForceGenerator::SpringForceGenerator(string name, double k, double restingLength, Particle* other, float duration) :
	ForceGenerator(name, duration), k(k), restingLength(restingLength), other(other) {}

// particle es la particula a la que aplicar la fuerza teniendo en cuenta la otra
void SpringForceGenerator::updateForce(Particle* particle, double t) {
	// distancia entre las dos particulas
	Vector3 relativePos = this->getPosOtherEnd() - particle->getPos();

	float length = relativePos.normalize();	// se devuelve la longitud del vector y luego, se normaliza
	// cuanto se ha deformado el muelle
	float deltaX = length - restingLength;

	// F = -k*x
	// k es la cte elastica del muelle
	// x es cuanto se ha deformado el muelle (longitud de deformacion)
	Vector3 force = k * relativePos * deltaX;
	particle->addForce(force);
}