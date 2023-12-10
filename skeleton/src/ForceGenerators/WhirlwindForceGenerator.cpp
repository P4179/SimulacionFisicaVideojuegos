#include "WhirlwindForceGenerator.h"

Vector3 WhirlwindForceGenerator::calculateWind(Particle* particle) {
	Vector3 pos = particle->getPos();
	float x = -(pos.z - center.p.z);
	float y = height - (pos.y - center.p.y);
	float z = pos.x - center.p.x;

	// si la partícula ha superado la altura del tornado deja de recibir la fuerza en y
	if (particle->getPos().y >= center.p.y + height) {
		return fuerzaTorbellino * Vector3(x, 0, z);
	}
	return fuerzaTorbellino * Vector3(x, y, z);
}

WhirlwindForceGenerator::WhirlwindForceGenerator(string name, Vector3 posInfluenceArea, float radiusInfluenceArea, float height, float fuerzaTorbellino, float duration, bool lastOption, bool show) :
	WindForceGenerator(name, vector<Vector3>(1), posInfluenceArea, radiusInfluenceArea, duration, lastOption, show),
	height(height), fuerzaTorbellino(fuerzaTorbellino), lastOption(lastOption) {}

void WhirlwindForceGenerator::updateForce(Particle* particle, double t) {
	if (isInInfluenceArea(particle)) {
		windVel[0] = calculateWind(particle);

		WindForceGenerator::updateForce(particle, t);
	}
}