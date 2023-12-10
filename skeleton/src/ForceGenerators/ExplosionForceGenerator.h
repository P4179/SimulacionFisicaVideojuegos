#pragma once
#include "ForceGenerator.h"
#define _USE_MATH_DEFINES
#include <math.h>

class ExplosionForceGenerator : public ForceGenerator {
private:
	const Vector4 INFLUENCE_AREA_COLOR = Vector4(0.741, 0.741, 0.741, 0.639);
	// m/s --> km/s (0.3432)
	const float SOUND_AIR_SPEED = 343.2;

	physx::PxTransform center;
	RenderItem* render;
	// radio de la explosion, va aumentando con el tiempo
	float radius;
	// radio original
	float originalRadius;
	float explosionIntensity;
	float explosionFadingTime;
	bool show;
	float elapsedTime;
	bool explode;

	bool isInInfluenceArea(Particle* particle);

public:
	ExplosionForceGenerator(string name, Vector3 centerInfluenceArea, float radiusInfluenceArea, float explosionIntensity, float explosionFadingTime, float duration = -1.0, bool show = false);

	virtual ~ExplosionForceGenerator();

	virtual void updateForce(Particle* particle, double t);

	void enableExplosion();
};