#pragma once
#include "ForceGenerator.h"

class BuoyancyForceGenerator : public ForceGenerator {
private:
	const float GRAVITY = 9.8;
	const float LIQUID_SIZE = 100;
	const Vector4 COLOR = Vector4(0.212, 0.643, 0.9, 1);
	float h0;	// altura del liquido
	float density;	// densidad del agua

public:
	BuoyancyForceGenerator(string name, float liquidPosY, float liquidHeight, float density, double duration = -1.0) :
		ForceGenerator(name, duration), h0(liquidPosY + liquidHeight), density(density) {

		// agua
		// si se crea directamente, se producen errores muy raros
		new Particle(Vector3(0, liquidPosY, 0), {}, {}, {}, {}, {}, Vector3(LIQUID_SIZE, liquidHeight, LIQUID_SIZE), COLOR);
	}

	virtual void updateForce(Particle* particle, double t) {
		float h = particle->getPos().y;	// altura de la particula
		float length = particle->getLength();	// longitud de la particula

		float inmmersed;	// porcentaje de inmersion del cubo
		// la particula no esta inmersa
		if (h - length * 0.5 - h0 > 0) {
			inmmersed = 0.0;
		}
		// la particula esta inmersa completamente
		else if (h + length * 0.5 - h0 < 0) {
			inmmersed = 1.0;
		}
		else {
			inmmersed = (h0 - h) / length + 0.5;
		}

		// Al introducir un objeto en un liquido habra un trozo del liquido que se quedara por fuera/desolojada
		// Entonces, este trozo del liquido aplicara una fuerza en sentido opuesto al peso del objeto
		// para volver a su posicion inicial
		// La fuerza que aplica es la fuerza de flotacion, que se basas en su peso
		// Fflotacion = PliqDesalojado = m * g	// g es la gravedad, 9.8
		// m = VliqDesalojado * d
		// El volumen del liquido desalojada corresponde con el volumen que hay del objeto dentro del agua
		// Por lo tanto, la formula queda:
		// Fflot = VliqDesalojado * d * g
		// SE ASEMEJA MUCHO A LA FORMULA DE UN MUELLE, ES POR ESO QUE SE TRATA DE UN CASO PARTICULAR
		// DE UN MUELLE

		Vector3 force = Vector3(0, particle->getVolume() * inmmersed * density * GRAVITY, 0);
		particle->addForce(force);
	}

};