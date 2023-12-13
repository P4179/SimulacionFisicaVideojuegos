#pragma once
#include "ForceGenerator.h"
#include <random>
// numero PI
#define _USE_MATH_DEFINES
#include <math.h>

class WindForceGenerator : public ForceGenerator<Particle> {
protected:
	// depende de la forma del objeto con el que impacto el viento
	// para objetos esfericos es 0.5
	const float COEF_ROZ_AIRE = 0.5;
	const float DENSIDAD_AIRE = 1.2;
	const Vector4 INFLUENCE_AREA_COLOR = Vector4(0.741, 0.741, 0.741, 0.639);

	std::uniform_int_distribution<int> dist;
	std::mt19937 mt;
	RenderItem* renderInfluenceArea;
	float radiusInfluenceArea;
	std::vector<Vector3> windVel;
	physx::PxTransform center;
	bool show;
	bool lastOption;

	bool isInInfluenceArea(Particle* particle);

public:
	WindForceGenerator(string name, const vector<Vector3>& windVel, Vector3 centerInfluenceArea, float radiusInfluenceArea, float duration = -1.0, bool lastOption = true, bool show = false);

	virtual ~WindForceGenerator();

	virtual void updateForce(Particle* particle, double t);
};