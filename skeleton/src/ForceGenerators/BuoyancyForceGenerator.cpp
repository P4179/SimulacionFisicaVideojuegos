#include "BuoyancyForceGenerator.h"
#include "../Particles/BoxParticle.h"
#include "../checkML.h"

BuoyancyForceGenerator::BuoyancyForceGenerator(string name, float liquidPosY, float liquidHeight, float density, LiquidVisual visual, double duration) :
	ForceGenerator(name, duration), h0(liquidPosY + liquidHeight), density(density) {

	// agua
	// si se crea directamente, se producen errores muy raros
	liquid = new BoxParticle(Vector3(visual.x, liquidPosY, visual.z), {}, {}, {}, {}, {}, Vector3(visual.size, liquidHeight, visual.size), visual.color);
}

BuoyancyForceGenerator::~BuoyancyForceGenerator() {
	delete liquid;
}

void BuoyancyForceGenerator::updateForce(Particle* particle, double t) {
	float h = particle->getPos().y;	// altura de la particula
	float length = particle->getLength();	// longitud de la particula

	float inmmersed;	// porcentaje de inmersion del cubo
	// la particula no esta inmersa
	if (h - length * 0.5 - h0 > 0) {
		particle->setImmersion(Particle::Out);
		inmmersed = 0.0;
	}
	// la particula esta inmersa completamente
	else if (h + length * 0.5 - h0 < 0) {
		particle->setImmersion(Particle::Floating);
		inmmersed = 1.0;
	}
	else {
		particle->setImmersion(Particle::Full);
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