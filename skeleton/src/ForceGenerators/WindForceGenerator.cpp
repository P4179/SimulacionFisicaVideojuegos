#include "WindForceGenerator.h"

bool WindForceGenerator::isInInfluenceArea(Particle* particle) {
	// si la distancia entre la particula y la esfera es menor o igual
	// que el radio de la esfera, se encuentra dentro del area de influencia
	Vector3 dif = particle->getPos() - center.p;
	return dif.magnitude() <= radiusInfluenceArea;
}

WindForceGenerator::WindForceGenerator(string name, const vector<Vector3>& windVel, Vector3 centerInfluenceArea, float radiusInfluenceArea, float duration, bool lastOption, bool show) :
	ForceGenerator(name, duration), windVel(windVel), dist(0, windVel.size() - 1),
	center(centerInfluenceArea.x, centerInfluenceArea.y, centerInfluenceArea.z),
	radiusInfluenceArea(radiusInfluenceArea), lastOption(lastOption), show(show) {
	std::random_device rd;
	mt = std::mt19937(rd());

	if (show) {
		physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(radiusInfluenceArea));
		renderInfluenceArea = new RenderItem(shape, &center, INFLUENCE_AREA_COLOR);
	}
}

WindForceGenerator::~WindForceGenerator() {
	if (show) {
		DeregisterRenderItem(renderInfluenceArea);
	}
}

void WindForceGenerator::updateForce(Particle* particle, double t) {
	if (isInInfluenceArea(particle)) {
		Vector3 dif = windVel[dist(mt)] - particle->getVel();
		float k = 0;
		if (lastOption) {
			// k verdaderamente depende del tamaño y de la forma del objeto y del ángulo de incidencia del viento
			// se puede calcular como k=A*COEF_ROZ_AIRE*p
			// A --> área de la sección transversal a la dirección de movimiento (PI*Diametro^2/4)
			// se aproxima a que es un cuarto del area de la esfera
			// p --> densidad del aire (1.2)
			float A = particle->getArea() / 4;
			k = A * COEF_ROZ_AIRE * DENSIDAD_AIRE;
		}
		Vector3 force = COEF_ROZ_AIRE * dif + k * dif.magnitude() * dif;
		particle->addForce(force);
	}
}