#pragma once
#include "ForceGenerator.h"

// dos fuerzas distintas por cada muelle
// una que mueve una particula y otra que mueve la otra particula
class SpringForceGenerator : public ForceGenerator {
protected:
	double k;
	// longitud del muelle cuando esta en reposo
	double restingLength;
	Particle* other;

public:
	SpringForceGenerator(string name, double k, double restingLength, Particle* other, float duration = -1.0) :
		ForceGenerator(name, duration), k(k), restingLength(restingLength), other(other) {}

	// particle es la particula a la que aplicar la fuerza teniendo en cuenta la otra
	virtual void updateForce(Particle* particle, double t) {
		// distancia entre las dos particulas
		Vector3 relativePos = this->getPosOtherEnd() - particle->getPos();

		float length = relativePos.normalize();	// se devuelve la longitud del vector y luego, se normaliza
		// cuanto se ha deformado el muelle
		float deltaX = length - restingLength;

		// F = -k*x
		// k es la cte elastica del muelle
		// x es cuanto se ha deformado el muelle (longitud de deformacion)
		Vector3 force = k * relativePos * deltaX;
	}

	virtual inline Vector3 getPosOtherEnd() const {
		return other->getPos();
	}
};