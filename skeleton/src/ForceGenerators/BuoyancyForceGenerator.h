#pragma once
#include "ForceGenerator.h"

class BuoyancyForceGenerator : public ForceGenerator<Particle> {
private:
	const float GRAVITY = 9.8;
	const float LIQUID_SIZE = 100;
	const Vector4 COLOR = Vector4(0.212, 0.643, 0.9, 1);
	float h0;	// altura del liquido
	float density;	// densidad del agua
	Particle* liquid;

public:
	BuoyancyForceGenerator(string name, float liquidPosY, float liquidHeight, float density, double duration = -1.0);

	virtual ~BuoyancyForceGenerator();

	virtual void updateForce(Particle* particle, double t);

};