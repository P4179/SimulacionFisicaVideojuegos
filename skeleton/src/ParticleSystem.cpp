#include "ParticleSystem.h"
#include "checkML.h"

ParticleSystem::ParticleSystem(Vector3 gravity) : gravity(gravity),
particle_generators(vector<std::pair<ParticleGenerator*, bool>>(MAX)), forceGenerators(MAX_FORCES), selectedGen(NONE) {

	// inicializar semilla de rand para que cada vez que se ejecute el programa
	// no salgan los mismo números
	srand(time(NULL));

	registry = new ParticleForceRegistry();
	particles = new ListParticles(1000, registry);

	generateForceGens();

	generateNormalGens();
}

void ParticleSystem::generateForceGens() {
	forceGenerators[GravityGen] = new GravityForceGenerator("GravityGen", gravity, -1);
	forceGenerators[WindGen] = new WindForceGenerator("WindGen", { Vector3(10, 0, 0), Vector3(15, 0, 0), Vector3(20, 0, 0) }, Vector3(0, 30, 0), 10, -1, true, false);
	forceGenerators[WhirlWindGen] = new WhirlwindForceGenerator("WhirlwindGen", Vector3(0, 0, 0), 800, 50, 50, 20, false, true);
	explosionGen = new ExplosionGenerator("ExplosionGen", Vector3(0, 50, 0), 20, 3000, 15, -1, false);
	forceGenerators[ExplosionGen] = explosionGen;

	ParticleInfo info;
	info.damping = DAMPING;

	info.invMasa = 0.02;
	info.color = Vector4(0.337, 0.192, 0.8, 1);
	info.lifeTime = 20;
	info.radius = 1.4;
	info.vSimulada = 45;
	// pos original, velocidad original, info particula, probabilidad, numero particulas, variacion velocidad, variacion posicion, generador de fuerzas
	vector<ForceGenerator*> gens = { forceGenerators[GravityGen] };
	addParticleGenerator<ForceParticleGenerator>(Gravedad, false, Vector3(0, 0, 0), Vector3(0, 1, 0), info, 0.3, 1, Vector3(0.2, 0, 0.2), Vector3(2, 0, 2), gens);

	info.invMasa = 0.035;
	info.color = Vector4(0.871, 0.804, 0.157, 1);
	info.lifeTime = 20;
	info.radius = 1.2;
	info.vSimulada = 45;
	// pos original, velocidad original, info particula, probabilidad, numero particulas, variacion velocidad, variacion posicion, generador de fuerzas
	gens = { forceGenerators[WindGen] };
	addParticleGenerator<ForceParticleGenerator>(Viento1, true, Vector3(0, 0, 0), Vector3(0, 1, 0), info, 0.3, 1, Vector3(0.2, 0, 0.2), Vector3(2, 0, 2), gens);

	info.invMasa = 0.015;
	info.color = Vector4(0.871, 0.569, 0.157, 1);
	info.lifeTime = 20;
	info.radius = 1.2;
	info.vSimulada = 45;
	// pos original, velocidad original, info particula, probabilidad, numero particulas, variacion velocidad, variacion posicion, generador de fuerzas
	gens = { forceGenerators[WindGen] };
	addParticleGenerator<ForceParticleGenerator>(Viento2, true, Vector3(0, 0, 0), Vector3(0, 1, 0), info, 0.3, 1, Vector3(0.2, 0, 0.2), Vector3(2, 0, 2), gens);

	info.invMasa = 0.03;
	info.color = Vector4(0.51, 0.749, 0.584, 1);
	info.lifeTime = 10;
	info.radius = 1.3;
	info.vSimulada = 40;
	// pos original, velocidad original, info particula, probabilidad, numero particulas, variacion velocidad, variacion posicion, generador de fuerzas
	gens = { forceGenerators[WhirlWindGen] };
	// LAS PARTÍCULAS NO SE MUEVEN, SINO QUE ES LA PROPIA FUERZA DEL TORBELLINO EL QUE HACE TODO EL MOVIMIENTO
	// EL GENERADOR SOLO COLAS PARTÍCULAS EN UNA ZONA Y LUEGO, LAS FUERZAS LAS MUEVE
	addParticleGenerator<ForceParticleGenerator>(Torbellino, false, Vector3(0, 0, 0), Vector3(0, 0, 0), info, 0.3, 1, Vector3(0, 0, 0), Vector3(10, 0, 10), gens);

	info.invMasa = 0.02;
	info.color = Vector4(0.851, 0.404, 0.282, 1);
	info.lifeTime = 8;
	info.radius = 1;
	info.vSimulada = 30;
	// pos original, velocidad original, info particula, probabilidad, numero particulas, variacion velocidad, variacion posicion, generador de fuerzas
	gens = { forceGenerators[ExplosionGen] };
	addParticleGenerator<ForceParticleGenerator>(Explosion, false, Vector3(0, 0, 0), Vector3(0, 1, 0), info, 0.3, 1, Vector3(0.2, 0, 0.2), Vector3(2, 0, 2), gens);
}

void ParticleSystem::generateNormalGens() {
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
	auto fireGen = addFireworkGenerator<FireworkGenerator>(Fire1, false, false, Vector3(0, 0, 0), Vector3(0, 1, 0), info, 2, Vector3(0.1, 0.2, 0.1), Vector3(1, 1, 1));

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

	for (auto& forceGen : forceGenerators) {
		delete forceGen;
	}
	forceGenerators.clear();

	delete registry;
}

// t es el deltatime
void ParticleSystem::integrate(double t) {
	particles->refresh();

	// update de cada partícula
	particles->integrate(t);

	// si se ha excedido el tiempo en el que actua la fuerza,
	// se saca del registro
	// esa fuerza se eliminara con todas
	registry->updateTime(t);

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
		selectNextGen(Fuente, Niebla);
		break;
	case 'C':
		registry->clear();
		changeActiveGen(Gravedad);
		break;
	case 'V':
		if (selectNextGen(Gravedad, Explosion)) {
			registry->clear();
		}
		break;
	case 'B':
		if (isGenActive(Explosion)) {
			explosionGen->enableExplosion();
		}
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