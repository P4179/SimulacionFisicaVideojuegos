#include "ParticleSystem.h"
#include "checkML.h"

ParticleSystem::ParticleSystem(Vector3 gravity) : gravity(gravity), //selectedGen(Circle),
particle_generators(vector<std::pair<ParticleGenerator*, bool>>(MAX)) {

	// inicializar semilla de rand para que cada vez que se ejecute el programa
	// no salgan los mismo números
	srand(time(NULL));

	particles = new ListParticles(1000);

	ParticleInfo info;
	info.ac = gravity;
	info.damping = DAMPING;

	info.color = Vector4(0.337, 0.192, 0.8, 1);
	info.lifeTime = 20;
	info.radius = 1.4;
	info.vSimulada = 45;
	// pos original, velocidad original, info particula, probabilidad, numero particulas, variacion velocidad, variacion posicion
	addParticleGenerator<UniformParticleGenerator>(Fuente, false, Vector3(0, 0, 0), Vector3(0, 1, 0), info, 0.3, 1, Vector3(0.2, 0, 0.2), Vector3(2, 0, 2));

	info.color = Vector4(0.18, 0.596, 0.839, 1);
	info.lifeTime = 20;
	info.radius = 1;
	info.vSimulada = 40.0;
	addParticleGenerator<UniformParticleGenerator>(Manguera, false, Vector3(-40, 10, 0), Vector3(0.6, 1, 0), info, 0.3, 1, Vector3(0, 0.3, 0.2), Vector3(1, 0, 1));

	info.color = Vector4(0.361, 0.529, 0.729, 1);
	info.lifeTime = 20;
	info.radius = 0.6;
	info.vSimulada = 35.0;
	addParticleGenerator<UniformParticleGenerator>(LLuvia, false, Vector3(0, 65, 0), Vector3(0, -1, 0), info, 0.5, 3, Vector3(0, 0, 0), Vector3(20, 2, 20));

	// si el alpha de la bola es 1, la pantalla se pone negra
	info.color = Vector4(0.596, 0.929, 0.855, 1);
	info.lifeTime = 20;
	info.radius = 0.8;
	info.vSimulada = 45.0;
	// la vel inicial y las direcciones de velocidad solo sirven para indicar cual incrementa más y cual incrementa menos
	// pues luego, se normalizara el vector y se multiplicar por la vSimulada
	mangueraGaussiana = addParticleGenerator<GaussianParticleGenerator>(MangueraGaussiana, false, Vector3(-60, 10, 0), Vector3(0.7, 0.4, 0), info, 0.5, 3, Vector3(0.03, 0.02, 0.05), Vector3(0, 0, 0.5));

	info.color = Vector4(0.518, 0.635, 0.678, 0.7);
	info.lifeTime = 20;
	info.radius = 0.7;
	info.vSimulada = 8;
	// la vel inicial y las direcciones de velocidad solo sirven para indicar cual incrementa más y cual incrementa menos
	// pues luego, se normalizara el vector y se multiplicar por la vSimulada
	addParticleGenerator<GaussianParticleGenerator>(Niebla, false, Vector3(0, 30, 0), Vector3(0, 1, 0), info, 0.7, 4, Vector3(2.5, 0.02, 2.5), Vector3(15, 12, 15));

	generateFireworkSystem();
}

void ParticleSystem::generateFireworkSystem() {
	ParticleInfo info;
	info.ac = gravity;
	info.damping = DAMPING;

	info.color = Vector4(0.89, 0.09, 0.29, 1);
	info.lifeTime = 1.8;
	info.radius = 1.3;
	info.vSimulada = 25.0;
	// pos original, velocidad original, info particula, numero particulas, variacion velocidad, variacion posicion
	auto fireGen = addFireworkGenerator<FireworkGenerator>(Fire1, true, false, Vector3(0, 0, 0), Vector3(0, 1, 0), info, 2, Vector3(0.1, 0.2, 0.1), Vector3(1, 1, 1));

	info.color = Vector4(0.196, 0.871, 0.153, 1);
	info.lifeTime = 2.3;
	info.radius = 0.9;
	info.vSimulada = 20.0;
	// pos original, velocidad original, info particula, numero particulas, variacion velocidad, variacion posicion
	fireGen = addFireworkGenerator<FireworkGenerator>(Fire2, false, true, Vector3(0, 20, 0), Vector3(0, 1, 0), info, 4, Vector3(0.8, 0.1, 0.8), Vector3(1, 1, 1));

	info.color = Vector4(0.612, 0.216, 0.91, 1);
	info.lifeTime = 2.9;
	info.radius = 1;
	info.vSimulada = 25.0;
	// pos original, velocidad original, info particula, numero particulas, variacion velocidad, variacion posicion
	fireGen = addFireworkGenerator<FireworkGenerator>(Fire3, false, true, Vector3(0, 20, 0), Vector3(0.8, 0.8, 0), info, 2, Vector3(0.3, 0, 0.5), Vector3(1, 1, 1));

	info.color = Vector4(0.961, 0.118, 0.808, 1);
	info.lifeTime = 2.7;
	info.radius = 0.8;
	info.vSimulada = 30.0;
	// pos original, velocidad original, info particula, numero particulas, variacion velocidad, variacion posicion
	fireGen = addFireworkGenerator<FireworkGenerator>(Fire4, false, true, Vector3(0, 20, 0), Vector3(-0.7, 0.6, 0), info, 3, Vector3(0.4, 0.2, 0.1), Vector3(1, 1, 1));

	info.color = Vector4(0.922, 0.906, 0.18, 1);
	info.lifeTime = 1.5;
	info.radius = 1.5;
	info.vSimulada = 35;
	try {
		// los dos últimos vectores, que definen el plano donde se crea el círculo deben ser perpendiculares y unitarios
		addFireworkGenerator<CircleGenerator>(Circle, false, false, Vector3(-10, 30, 0), info, 20, Vector3(0.1, 0.1, 0), Vector3(0, 1, 0), Vector3(1, 0, 0));
	}
	catch (exception e) {
		cout << "It could not be created a circle generator" << "\n";
	}
}

ParticleSystem::~ParticleSystem() {
	delete particles;
	for (auto& gen : particle_generators) {
		delete gen.first;
	}
	particle_generators.clear();
}

void ParticleSystem::integrate(double t) {
	particles->refresh();

	// update de cada partícula
	particles->integrate(t);

	// generar más partículas
	for (auto generator : particle_generators) {
		if (generator.second) {
			generator.first->update(particles);
		}
	}
}

void ParticleSystem::keyPressed(int __cdecl key) {
	switch (key) {
	case 'Z':
		changeActiveGen(Fuente);
		break;
	case 'X':
		changeActiveGen(LLuvia);
		break;
	case 'C':
		changeActiveGen(Manguera);
		break;
	case 'V':
		changeActiveGen(MangueraGaussiana);
		break;
	case 'B':
		changeActiveGen(Niebla);
		break;
	case 'N':
		changeActiveGen(Fire1);
		break;
	case 'M':
		changeActiveGen(Circle);
		break;
	case 'H':
		if (isGenActive(MangueraGaussiana)) {
			getParticleGenerator(MangueraGaussiana)->decreaseSimulatedV();
		}
		break;
	case 'J':
		if (isGenActive(MangueraGaussiana)) {
			getParticleGenerator(MangueraGaussiana)->increaseSimulatedV();
		}
		break;
	case 'K':
		if (isGenActive(MangueraGaussiana)) {
			auto stdDevVel = mangueraGaussiana->getStdDevVel();
			stdDevVel.z -= 0.1;
			if (stdDevVel.z < 0) {
				stdDevVel.z = 0;
			}
			mangueraGaussiana->setStdDevVel(stdDevVel);
		}
		break;
	case 'L':
		if (isGenActive(MangueraGaussiana)) {
			auto stdDevVel = mangueraGaussiana->getStdDevVel();
			stdDevVel.z += 0.1;
			mangueraGaussiana->setStdDevVel(stdDevVel);
		}
		break;
	}
}