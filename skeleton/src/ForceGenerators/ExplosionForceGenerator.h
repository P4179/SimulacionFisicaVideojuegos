#pragma once
#include "ForceGenerator.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <climits>

template <typename T>
class ExplosionForceGenerator : public ForceGenerator<T> {
private:
	const Vector4 INFLUENCE_AREA_COLOR = Vector4(0.741, 0.741, 0.741, 0.639);
	// m/s --> km/s (0.3432)
	const float SOUND_AIR_SPEED = 343.2;

	physx::PxTransform center;
	RenderItem* render;
	// radio de la explosion, va aumentando con el tiempo
	int radius;
	int maxRadius;
	// radio original
	float originalRadius;
	float explosionIntensity;
	float explosionFadingTime;
	bool show;
	float elapsedTime;
	bool canExplode;
	bool expansion;

	bool isInInfluenceArea(T* particle) {
		float distExplosion = (particle->getPos() - center.p).magnitude();
		bool aux = distExplosion < radius;
		return aux;
	}

public:
	ExplosionForceGenerator(string name, Vector3 centerInfluenceArea, int radiusInfluenceArea, float explosionIntensity,
		float explosionFadingTime, float duration = -1.0, bool show = false, bool expansion = true, int maxRadius = INT_MAX) :
		ForceGenerator(name, duration),
		center(centerInfluenceArea.x, centerInfluenceArea.y, centerInfluenceArea.z),
		radius(0), originalRadius(radiusInfluenceArea), explosionIntensity(explosionIntensity),
		explosionFadingTime(explosionFadingTime), elapsedTime(0), canExplode(false), expansion(expansion),
		show(show), maxRadius(maxRadius) {

		if (show) {
			physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(originalRadius));
			render = new RenderItem(shape, &center, INFLUENCE_AREA_COLOR);
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

	virtual void updateForce(T* particle, double t) {
		if (canExplode) {
			elapsedTime += t;
			radius = originalRadius;
			if (expansion) {
				radius += SOUND_AIR_SPEED * elapsedTime;
				if (radius > maxRadius) {
					radius = maxRadius;
				}
				else if (show) {
					DeregisterRenderItem(render);
					physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(radius));
					render = new RenderItem(shape, &center, INFLUENCE_AREA_COLOR);
				}
			}
			// la partícula se encuentra dentro del area de influencia
			if (isInInfluenceArea(particle)) {
				Vector3 dif = particle->getPos() - center.p;
				float r = dif.magnitude();
				// a partir de 4 * explosionFadingTime, la explosion comienza a desaparecer
				// exp se refiere a e^(...)
				Vector3 force = (explosionIntensity / (r * r)) * dif * exp(-elapsedTime / explosionFadingTime);
				particle->addForce(force);
			}
		}
	}

	void explode() {
		// activar explosion
		canExplode = true;
		// reiniciar explosion
		elapsedTime = 0;
		radius = originalRadius;
		// para que se vea mas claro
		if (show) {
			DeregisterRenderItem(render);
			physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(radius));
			render = new RenderItem(shape, &center, INFLUENCE_AREA_COLOR);
		}
	}

	void disableExplosion() {
		// desactivar explosion
		canExplode = false;
		radius = originalRadius;
		if (show) {
			DeregisterRenderItem(render);
			physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(radius));
			render = new RenderItem(shape, &center, INFLUENCE_AREA_COLOR);
		}
		// se reinicia por si acaso
		elapsedTime = 0;
	}
};