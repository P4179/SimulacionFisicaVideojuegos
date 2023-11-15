#pragma once
#include "ForceGenerator.h"

class GravityForceGenerator : public ForceGenerator {
private:
	Vector3 gravity;

public:
	GravityForceGenerator(string name, const Vector3& gravity, double duration = -1.0) :
		ForceGenerator(name, duration), gravity(gravity) {}

	virtual void updateForce(Particle* particle, double t) {
		try {
			// formula de la aceleracion de la gravedad simplificada
			// suponemos masa y radio de la tierra como constantes
			float masa = particle->getMasa();
			Vector3 force = gravity * masa;
			particle->addForce(force);
		}
		// si la masa del objeto es infinita, no se le aplican fuerzas
		catch (exception e) {
			return;
		}
	}
};