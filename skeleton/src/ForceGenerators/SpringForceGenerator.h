#pragma once
#include "ForceGenerator.h"

// dos fuerzas distintas por cada muelle
// una que mueve una particula y otra que mueve la otra particula
class SpringForceGenerator : public ForceGenerator {
protected:
	// constante elastica del muelle
	// determina la intensidad de la fuerza
	double k;
	// longitud del muelle cuando esta en reposo (no se ha deformado)
	double restingLength;
	Particle* other;

public:
	SpringForceGenerator(string name, double k, double restingLength, Particle* other, float duration = -1.0);

	// particle es la particula a la que aplicar la fuerza teniendo en cuenta la otra
	virtual void updateForce(Particle* particle, double t);

	virtual inline Vector3 getPosOtherEnd() const {
		return other->getPos();
	}

	inline void decreaseK() {
		--k;
		if (k < 0) {
			k = 0;
		}
		cout << this->getName() <<": K = " + to_string((int)k) << "\n";
	}

	inline void increaseK() {
		++k;
		cout << "K: " + to_string((int)k) << "\n";
		cout << this->getName() << ": K = " + to_string((int)k) << "\n";
	}
};