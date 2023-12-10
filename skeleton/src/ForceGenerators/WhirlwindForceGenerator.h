#pragma once
#include "WindForceGenerator.h"

class WhirlwindForceGenerator : public WindForceGenerator {
private:
	bool lastOption;
	int height;
	float fuerzaTorbellino;

	Vector3 calculateWind(Particle* particle);

public:
	WhirlwindForceGenerator(string name, Vector3 posInfluenceArea, float radiusInfluenceArea, float height, float fuerzaTorbellino, float duration = -1.0, bool lastOption = false, bool show = false);

	virtual void updateForce(Particle* particle, double t);
};