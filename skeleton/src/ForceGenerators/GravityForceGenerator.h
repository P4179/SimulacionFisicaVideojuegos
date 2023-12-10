#pragma once
#include "ForceGenerator.h"

class GravityForceGenerator : public ForceGenerator {
private:
	Vector3 gravity;

public:
	GravityForceGenerator(string name, const Vector3& gravity, double duration = -1.0);

	virtual void updateForce(Particle* particle, double t);
};