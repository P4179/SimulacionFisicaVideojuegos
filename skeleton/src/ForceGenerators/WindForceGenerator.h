#pragma once
#include "ForceGenerator.h"
#include <random>
// numero PI
#define _USE_MATH_DEFINES
#include <math.h>

class WindForceGenerator : public ForceGenerator {
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

	bool isInInfluenceArea(Particle* particle) {
		// si la distancia entre la particula y la esfera es menor o igual
		// que el radio de la esfera, se encuentra dentro del area de influencia
		Vector3 dif = particle->getPos() - center.p;
		return dif.magnitude() <= radiusInfluenceArea;
	}

public:
	WindForceGenerator(string name, const vector<Vector3>& windVel, Vector3 centerInfluenceArea, float radiusInfluenceArea, float duration = -1.0, bool lastOption = true, bool show = false) :
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

	virtual ~WindForceGenerator() {
		if (show) {
			DeregisterRenderItem(renderInfluenceArea);
		}
	}

	virtual void updateForce(Particle* particle, double t) {
		if (isInInfluenceArea(particle)) {
			Vector3 dif = windVel[dist(mt)] - particle->getVel();
			float k = 0;
			if (lastOption) {
				// k verdaderamente depende del tamaño y de la forma del objeto y del ángulo de incidencia del viento
				// se puede calcular como k=A*COEF_ROZ_AIRE*p
				// A --> área de la sección transversal a la dirección de movimiento, que es PI*Diametro^2/4
				// p --> densidad del aire (1.2)
				float A = M_PI * (2 * particle->getRadius()) * (2 * particle->getRadius()) / 4;
				k = A * COEF_ROZ_AIRE * DENSIDAD_AIRE;
			}
			Vector3 force = COEF_ROZ_AIRE * dif + k * dif.magnitude() * dif;
			particle->addForce(force);
		}
	}
};