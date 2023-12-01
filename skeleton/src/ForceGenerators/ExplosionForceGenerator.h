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

	bool isInInfluenceArea(Particle* particle) {
		float distExplosion = (particle->getPos() - center.p).magnitude();
		return distExplosion < radius;
	}

public:
	ExplosionForceGenerator(string name, Vector3 centerInfluenceArea, float radiusInfluenceArea, float explosionIntensity, float explosionFadingTime, float duration = -1.0, bool show = false) :
		ForceGenerator(name, duration),
		center(centerInfluenceArea.x, centerInfluenceArea.y, centerInfluenceArea.z),
		radius(0), originalRadius(radiusInfluenceArea), explosionIntensity(explosionIntensity),
		explosionFadingTime(explosionFadingTime), elapsedTime(0), explode(false) {

		if (show) {
			physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(radiusInfluenceArea));
			render = new RenderItem(shape, &center, INFLUENCE_AREA_COLOR);
			shape->release();
		}
		else {
			render = nullptr;
		}
	}

	virtual ~ExplosionForceGenerator() {
		if (show && render != nullptr) {
			DeregisterRenderItem(render);
		}
	}

	virtual void updateForce(Particle* particle, double t) {
		cout << show << "\n";
		if (explode) {
			elapsedTime += t;
			radius = originalRadius + SOUND_AIR_SPEED * elapsedTime;
			// la partícula se encuentra dentro del area de influencia
			if (isInInfluenceArea(particle)) {
				Vector3 dif = particle->getPos() - center.p;
				float r = dif.magnitude();
				// a partir de 4 * explosionFadingTime, la explosion comienza a desaparecer
				// exp se refiere a e^(...)
				Vector3 force = (explosionIntensity / r * r) * dif * exp(-elapsedTime / explosionFadingTime);
				particle->addForce(force);
			}
		}
	}

	void enableExplosion() {
		explode = true;
		elapsedTime = 0;
		radius = originalRadius;
	}
};