#include "ExplosionForceGenerator.h"

bool ExplosionForceGenerator::isInInfluenceArea(Particle* particle) {
	float distExplosion = (particle->getPos() - center.p).magnitude();
	return distExplosion < radius;
}

ExplosionForceGenerator::ExplosionForceGenerator(string name, Vector3 centerInfluenceArea, float radiusInfluenceArea, float explosionIntensity, float explosionFadingTime, float duration, bool show) :
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

ExplosionForceGenerator::~ExplosionForceGenerator() {
	if (show && render != nullptr) {
		DeregisterRenderItem(render);
	}
}

void ExplosionForceGenerator::updateForce(Particle* particle, double t) {
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

void ExplosionForceGenerator::enableExplosion() {
	explode = true;
	elapsedTime = 0;
	radius = originalRadius;
}