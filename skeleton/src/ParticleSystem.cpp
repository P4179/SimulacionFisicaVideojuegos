#include "ParticleSystem.h"
#include "checkML.h"

ParticleSystem::ParticleSystem(Vector3 gravity) : gravity(gravity),
particle_generators(vector<std::pair<ParticleGenerator*, bool>>(MAX, { nullptr, false })), 
forceGenerators(MAX_FORCES, nullptr), selectedGen(NONE), springFg(nullptr) {

	// inicializar semilla de rand para que cada vez que se ejecute el programa
	// no salgan los mismo números
	srand(time(NULL));

	registry = new ParticleForceRegistry<Particle>();
	particles = new ListParticles(1000, registry);

	forceGenerators[GravityFg] = new GravityForceGenerator("GravityFg", gravity, -1);
	forceGenerators[WindFg] = new WindForceGenerator("WindFg", { Vector3(10, 0, 0), Vector3(15, 0, 0), Vector3(20, 0, 0) }, Vector3(0, 30, 0), 10, -1, true, false);
	forceGenerators[WhirlWindFg] = new WhirlwindForceGenerator("WhirlwindFg", Vector3(0, 0, 0), 800, 50, 50, 20, false, false);
	explosionFg = new ExplosionForceGenerator<Particle>("ExplosionFg", Vector3(0, 50, 0), 20, 300000, 15, -1, false);
	forceGenerators[ExplosionFg] = explosionFg;

	generateForceGens();

	generateNormalGens();

	// nunca se va a iniciar
	// solo se guarda para utilizar a posteriori
	launcherParticleGen = addParticleGenerator<LauncherParticleGen>(LaunchGen, false);
}

void ParticleSystem::generateForceGens() {
	ParticleInfo info;
	info.damping = DAMPING;
	info.geometry = physx::PxGeometryType::eSPHERE;

	info.sphere_data.radius = 1.4;
	info.color = Vector4(0.337, 0.192, 0.8, 1);
	info.lifeTime = 8;
	info.renderParticle_data.invMasa = 0.02;
	info.renderParticle_data.vSimulada = 45;
	// pos original, velocidad original, info particula, probabilidad, numero particulas, variacion velocidad, variacion posicion, generador de fuerzas
	aplicarFuerzaGen = addParticleGenerator<UniformParticleGenerator>(AplicarFuerzaGen, false, Vector3(0, 0, 0), Vector3(0, 1, 0), info, 0.3, 1, Vector3(0.2, 0, 0.2), Vector3(2, 0, 2));
	cambiarFuerzasGen = aplicarFuerzaGen;	// se setea por si se lanzara este generador desde el principio

	info.sphere_data.radius = 1.2;
	info.color = Vector4(0.871, 0.804, 0.157, 1);
	info.lifeTime = 20;
	info.renderParticle_data.invMasa = 0.035;
	info.renderParticle_data.vSimulada = 45;
	// pos original, velocidad original, info particula, probabilidad, numero particulas, variacion velocidad, variacion posicion, generador de fuerzas
	auto gens = { forceGenerators[WindFg] };
	addParticleGenerator<UniformParticleGenerator>(Viento1Gen, false, Vector3(0, 0, 0), Vector3(0, 1, 0), info, 0.3, 1, Vector3(0.2, 0, 0.2), Vector3(2, 0, 2), gens);

	info.sphere_data.radius = 1.2;
	info.color = Vector4(0.871, 0.569, 0.157, 1);
	info.lifeTime = 20;
	info.renderParticle_data.invMasa = 0.015;
	info.renderParticle_data.vSimulada = 45;
	// pos original, velocidad original, info particula, probabilidad, numero particulas, variacion velocidad, variacion posicion, generador de fuerzas
	gens = { forceGenerators[WindFg] };
	addParticleGenerator<UniformParticleGenerator>(Viento2Gen, false, Vector3(0, 0, 0), Vector3(0, 1, 0), info, 0.3, 1, Vector3(0.2, 0, 0.2), Vector3(2, 0, 2), gens);

	info.sphere_data.radius = 1.3;
	info.color = Vector4(0.51, 0.749, 0.584, 1);
	info.lifeTime = 10;
	info.renderParticle_data.invMasa = 0.03;
	info.renderParticle_data.vSimulada = 40;
	// pos original, velocidad original, info particula, probabilidad, numero particulas, variacion velocidad, variacion posicion, generador de fuerzas
	gens = { forceGenerators[WhirlWindFg] };
	// LAS PARTÍCULAS NO SE MUEVEN, SINO QUE ES LA PROPIA FUERZA DEL TORBELLINO EL QUE HACE TODO EL MOVIMIENTO
	// EL GENERADOR SOLO COLAS PARTÍCULAS EN UNA ZONA Y LUEGO, LAS FUERZAS LAS MUEVE
	addParticleGenerator<UniformParticleGenerator>(TorbellinoGen, false, Vector3(0, 0, 0), Vector3(0, 0, 0), info, 0.3, 1, Vector3(0, 0, 0), Vector3(10, 0, 10), gens);

	info.sphere_data.radius = 1;
	info.color = Vector4(0.851, 0.404, 0.282, 1);
	info.lifeTime = 8;
	info.renderParticle_data.invMasa = 0.02;
	info.renderParticle_data.vSimulada = 30;
	// pos original, velocidad original, info particula, probabilidad, numero particulas, variacion velocidad, variacion posicion, generador de fuerzas
	gens = { forceGenerators[ExplosionFg] };
	addParticleGenerator<UniformParticleGenerator>(ExplosionGen, false, Vector3(0, 0, 0), Vector3(0, 1, 0), info, 0.3, 1, Vector3(0.2, 0, 0.2), Vector3(2, 0, 2), gens);
}

void ParticleSystem::generateNormalGens() {
	auto gens = { forceGenerators[GravityFg] };

	ParticleInfo info;
	//info.ac = gravity;	Se cambio para que tb utilizaran fuerzas
	info.damping = DAMPING;
	info.renderParticle_data.invMasa = 0.01;
	info.geometry = physx::PxGeometryType::eSPHERE;

	info.color = Vector4(0.337, 0.192, 0.8, 1);
	info.lifeTime = 20;
	info.sphere_data.radius = 1.4;
	info.renderParticle_data.vSimulada = 45;
	// pos original, velocidad original, info particula, probabilidad, numero particulas, variacion velocidad, variacion posicion
	addParticleGenerator<UniformParticleGenerator>(FuenteGen, false, Vector3(0, 0, 0), Vector3(0, 1, 0), info, 0.3, 1, Vector3(0.2, 0, 0.2), Vector3(2, 0, 2), gens);

	info.color = Vector4(0.18, 0.596, 0.839, 1);
	info.lifeTime = 20;
	info.sphere_data.radius = 1;
	info.renderParticle_data.vSimulada = 40.0;
	addParticleGenerator<UniformParticleGenerator>(MangueraGen, false, Vector3(-40, 10, 0), Vector3(0.6, 1, 0), info, 0.3, 1, Vector3(0, 0.3, 0.2), Vector3(1, 0, 1), gens);

	info.color = Vector4(0.361, 0.529, 0.729, 1);
	info.lifeTime = 20;
	info.sphere_data.radius = 0.6;
	info.renderParticle_data.vSimulada = 35.0;
	addParticleGenerator<UniformParticleGenerator>(LLuviaGen, true, Vector3(0, 65, 0), Vector3(0, -1, 0), info, 0.5, 3, Vector3(0, 0, 0), Vector3(20, 2, 20), gens);

	// si el alpha de la bola es 1, la pantalla se pone negra
	info.color = Vector4(0.596, 0.929, 0.855, 1);
	info.lifeTime = 20;
	info.sphere_data.radius = 0.8;
	info.renderParticle_data.vSimulada = 45.0;
	// la vel inicial y las direcciones de velocidad solo sirven para indicar cual incrementa más y cual incrementa menos
	// pues luego, se normalizara el vector y se multiplicar por la vSimulada
	mangueraGaussianGen = addParticleGenerator<GaussianParticleGenerator>(MangueraGaussianaGen, false, Vector3(-60, 10, 0), Vector3(0.7, 0.4, 0), info, 0.5, 3, Vector3(0.03, 0.02, 0.05), Vector3(0, 0, 0.5), gens);

	info.color = Vector4(0.518, 0.635, 0.678, 0.7);
	info.lifeTime = 20;
	info.sphere_data.radius = 0.7;
	info.renderParticle_data.vSimulada = 8;
	// la vel inicial y las direcciones de velocidad solo sirven para indicar cual incrementa más y cual incrementa menos
	// pues luego, se normalizara el vector y se multiplicar por la vSimulada
	addParticleGenerator<GaussianParticleGenerator>(NieblaGen, false, Vector3(0, 30, 0), Vector3(0, 1, 0), info, 0.7, 4, Vector3(2.5, 0.02, 2.5), Vector3(15, 12, 15), gens);

	generateFireworkSystem();
}

void ParticleSystem::generateFireworkSystem() {
	auto gens = { forceGenerators[GravityFg] };

	ParticleInfo info;
	//info.ac = gravity;	Se cambio para que tb utilizaran fuerzas
	info.renderParticle_data.invMasa = 0.2;
	info.damping = DAMPING;

	info.color = Vector4(0.89, 0.09, 0.29, 1);
	info.lifeTime = 1.8;
	info.sphere_data.radius = 1.3;
	info.renderParticle_data.vSimulada = 25.0;
	// pos original, velocidad original, info particula, numero particulas, variacion velocidad, variacion posicion
	auto fireGen = addFireworkGenerator<FireworkGenerator>(Fire1Gen, false, false, Vector3(0, 0, 0), Vector3(0, 1, 0), info, 2, Vector3(0.1, 0.2, 0.1), Vector3(1, 1, 1), gens);

	info.color = Vector4(0.196, 0.871, 0.153, 1);
	info.lifeTime = 2.3;
	info.sphere_data.radius = 0.9;
	info.renderParticle_data.vSimulada = 20.0;
	// pos original, velocidad original, info particula, numero particulas, variacion velocidad, variacion posicion
	fireGen = addFireworkGenerator<FireworkGenerator>(Fire2Gen, false, true, Vector3(0, 20, 0), Vector3(0, 1, 0), info, 4, Vector3(0.8, 0.1, 0.8), Vector3(1, 1, 1), gens);

	info.color = Vector4(0.612, 0.216, 0.91, 1);
	info.lifeTime = 2.9;
	info.sphere_data.radius = 1;
	info.renderParticle_data.vSimulada = 25.0;
	// pos original, velocidad original, info particula, numero particulas, variacion velocidad, variacion posicion
	fireGen = addFireworkGenerator<FireworkGenerator>(Fire3Gen, false, true, Vector3(0, 20, 0), Vector3(0.8, 0.8, 0), info, 2, Vector3(0.3, 0, 0.5), Vector3(1, 1, 1), gens);

	info.color = Vector4(0.961, 0.118, 0.808, 1);
	info.lifeTime = 2.7;
	info.sphere_data.radius = 0.8;
	info.renderParticle_data.vSimulada = 30.0;
	// pos original, velocidad original, info particula, numero particulas, variacion velocidad, variacion posicion
	fireGen = addFireworkGenerator<FireworkGenerator>(Fire4Gen, false, true, Vector3(0, 20, 0), Vector3(-0.7, 0.6, 0), info, 3, Vector3(0.4, 0.2, 0.1), Vector3(1, 1, 1), gens);

	info.color = Vector4(0.922, 0.906, 0.18, 1);
	info.lifeTime = 1.5;
	info.sphere_data.radius = 1.5;
	info.renderParticle_data.vSimulada = 35;
	try {
		// los dos últimos vectores, que definen el plano donde se crea el círculo deben ser perpendiculares y unitarios
		addFireworkGenerator<CircleGenerator>(CircleGen, false, false, Vector3(-10, 30, 0), info, 20, Vector3(0.1, 0.1, 0), Vector3(0, 1, 0), Vector3(1, 0, 0), gens);
	}
	catch (exception e) {
		cout << "It could not be created a circle generator" << "\n";
	}
}

void ParticleSystem::generateAnchorSystem(vector<std::pair<ForceGenerator<Particle>*, Particle*>>& forceParticles, unordered_set<ForceGenerator<Particle>*>& forceGens) {
	Particle* particle1 = new Particle(Vector3(-30, 20, 0), Vector3(0, 0, 0), 0.2, DAMPING, -1, 0);
	Particle* particle2 = new Particle(Vector3(30, 30, 0), Vector3(0, 0, 0), 0.1, DAMPING, -1, 0, 4, Vector4(0.322, 0.702, 0.361, 1));
	springFg = new AnchoredSpringForceGenerator("AnchoredSpringFg", 700, 20, Vector3(0, 50, 0));
	forceGenerators[AnchorFg] = springFg;
	forceParticles.push_back({ forceGenerators[AnchorFg], particle1 });
	forceParticles.push_back({ forceGenerators[AnchorFg], particle2 });

	forceGens.insert(forceGenerators[GravityFg]);
}

void ParticleSystem::generateSpringSystem(vector<std::pair<ForceGenerator<Particle>*, Particle*>>& forceParticles, unordered_set<ForceGenerator<Particle>*>& forceGens) {
	Particle* particle1 = new Particle(Vector3(-30, 70, 0), Vector3(0, 0, 0), 0.2, DAMPING, -1, 0);
	Particle* particle2 = new Particle(Vector3(30, 70, 0), Vector3(0, 0, 0), 0.08, DAMPING, -1, 0, 4, Vector4(0.322, 0.702, 0.361, 1));

	forceGenerators[Spring1Fg] = new SpringForceGenerator("Spring1Fg", 1500, 30, particle2);
	forceParticles.push_back({ forceGenerators[Spring1Fg], particle1 });
	springFg = new SpringForceGenerator("Spring2Fg", 700, 30, particle1);
	forceGenerators[Spring2Fg] = springFg;
	forceParticles.push_back({ forceGenerators[Spring2Fg], particle2 });

	forceGens.insert(forceGenerators[GravityFg]);
}

void ParticleSystem::generateElasticRubberSystem(vector<std::pair<ForceGenerator<Particle>*, Particle*>>& forceParticles, unordered_set<ForceGenerator<Particle>*>& forceGens) {
	Particle* particle1 = new Particle(Vector3(-40, 65, 0), Vector3(0, 0, 0), 0.2, DAMPING, -1, 0, 2.5, Vector4(0.812, 0.365, 0.765, 1));
	Particle* particle2 = new Particle(Vector3(40, 65, 0), Vector3(0, 0, 0), 0.1, DAMPING, -1, 0, 4, Vector4(0.322, 0.702, 0.361, 1));

	forceGenerators[ElasticRubber1Fg] = new ElasticRubberForceGenerator("ElasticRubber1Fg", 650, 40, particle2);
	forceParticles.push_back({ forceGenerators[ElasticRubber1Fg], particle1 });
	springFg = new ElasticRubberForceGenerator("ElasticRubber2Fg", 700, 40, particle1);
	forceGenerators[ElasticRubber2Fg] = springFg;
	forceParticles.push_back({ forceGenerators[ElasticRubber2Fg], particle2 });

	forceGens.insert(forceGenerators[GravityFg]);
}

void ParticleSystem::generateSlinkySystem(vector<std::pair<ForceGenerator<Particle>*, Particle*>>& forceParticles, unordered_set<ForceGenerator<Particle>*>& forceGens) {
	Particle* particle1 = new Particle(Vector3(0, 75, 0), Vector3(0, 0, 0), 0.2, DAMPING, -1, 0, 2, Vector4(0.58, 0, 0.83, 1));
	Particle* particle2 = new Particle(Vector3(0, 60, 0), Vector3(0, 0, 0), 0.15, DAMPING, -1, 0, 3, Vector4(0.29, 0, 0.51, 1));
	Particle* particle3 = new Particle(Vector3(0, 45, 0), Vector3(0, 0, 0), 0.2, DAMPING, -1, 0, 2, Vector4(0, 0, 1, 1));
	Particle* particle4 = new Particle(Vector3(0, 30, 0), Vector3(0, 0, 0), 0.15, DAMPING, -1, 0, 3, Vector4(0, 1, 0, 1));
	Particle* particle5 = new Particle(Vector3(0, 15, 0), Vector3(0, 0, 0), 0.2, DAMPING, -1, 0, 2, Vector4(1, 1, 0, 1));
	Particle* particle6 = new Particle(Vector3(0, 0, 0), Vector3(0, 0, 0), 0.15, DAMPING, -1, 0, 3, Vector4(1, 0.498, 0, 1));

	// particula 1-2
	forceGenerators[Slinky1Fg] = new SpringForceGenerator("Slink1Fg", 600, 10, particle2);
	forceParticles.push_back({ forceGenerators[Slinky1Fg], particle1 });
	forceGenerators[Slinky1BisFg] = new SpringForceGenerator("Slink1BisFg", 600, 10, particle1);
	forceParticles.push_back({ forceGenerators[Slinky1BisFg], particle2 });

	// particula 2-3
	forceGenerators[Slinky2Fg] = new SpringForceGenerator("Slink2Fg", 700, 10, particle3);
	forceParticles.push_back({ forceGenerators[Slinky2Fg], particle2 });
	forceGenerators[Slinky2BisFg] = new SpringForceGenerator("Slink2BisFg", 700, 10, particle2);
	forceParticles.push_back({ forceGenerators[Slinky2BisFg], particle3 });

	// particula 3-4
	forceGenerators[Slinky3Fg] = new SpringForceGenerator("Slink3Fg", 800, 10, particle4);
	forceParticles.push_back({ forceGenerators[Slinky3Fg], particle3 });
	forceGenerators[Slinky3BisFg] = new SpringForceGenerator("Slink3BisFg", 800, 10, particle3);
	forceParticles.push_back({ forceGenerators[Slinky3BisFg], particle4 });

	// particula 4-5
	forceGenerators[Slinky4Fg] = new SpringForceGenerator("Slink4Fg", 900, 10, particle5);
	forceParticles.push_back({ forceGenerators[Slinky4Fg], particle4 });
	forceGenerators[Slinky4BisFg] = new SpringForceGenerator("Slink4BisFg", 900, 10, particle4);
	forceParticles.push_back({ forceGenerators[Slinky4BisFg], particle5 });

	// particula 5-6
	forceGenerators[Slinky5Fg] = new SpringForceGenerator("Slink5Fg", 1000, 10, particle6);
	forceParticles.push_back({ forceGenerators[Slinky5Fg], particle5 });
	forceGenerators[Slinky5BisFg] = new SpringForceGenerator("Slink5BisFg", 1000, 10, particle5);
	forceParticles.push_back({ forceGenerators[Slinky5BisFg], particle6 });

	forceGens.insert(forceGenerators[GravityFg]);
}

void ParticleSystem::generateBuoyancySystem(vector<std::pair<ForceGenerator<Particle>*, Particle*>>& forceParticles, unordered_set<ForceGenerator<Particle>*>& forceGens) {
	Particle* particle = new BoxParticle(Vector3(0, 100, 0), Vector3(0, 0, 0), 0.0001, DAMPING, -1, 0, Vector3(8, 8, 8));
	forceGenerators[BuoyanceFg] = new BuoyancyForceGenerator("BuoyanceFg", 0, 30, WATER_DENSITY);
	forceParticles.push_back({ forceGenerators[BuoyanceFg], particle });

	forceGens.insert(forceGenerators[GravityFg]);
}

ParticleSystem::~ParticleSystem() {
	delete particles;

	for (auto& gen : particle_generators) {
		delete gen.first;
	}
	particle_generators.clear();

	for (auto& forceGen : forceGenerators) {
		if (forceGen != nullptr) {
			delete forceGen;
		}
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

/*
* ocupadas
* 1, 2, 3, 4, 5, 7, 8, 9, 0, O, P		PRACTICA 4	
* Z, X, C								PRACTICA 3
* V, B, N, M, J, K, L, M				PRACTICA 2
* T, Y, U								CAMBIAR FUERZAS
* 
*/
void ParticleSystem::keyPressed(int __cdecl key) {
	switch (key) {
		// muelle anclado a un objeto estatico
	case '1':
		launch([this](vector<std::pair<ForceGenerator<Particle>*, Particle*>>& forceParticles, unordered_set<ForceGenerator<Particle>*>& forceGens) {
			generateAnchorSystem(forceParticles, forceGens);
			});
		break;
		// particulas unidas mediante un muelle
	case '2':
		launch([this](vector<std::pair<ForceGenerator<Particle>*, Particle*>>& forceParticles, unordered_set<ForceGenerator<Particle>*>& forceGens) {
			generateSpringSystem(forceParticles, forceGens);
			});
		break;
		// goma elastica
	case '3':
		launch([this](vector<std::pair<ForceGenerator<Particle>*, Particle*>>& forceParticles, unordered_set<ForceGenerator<Particle>*>& forceGens) {
			generateElasticRubberSystem(forceParticles, forceGens);
			});
		break;
		// flotacion
	case '5':
		launch([this](vector<std::pair<ForceGenerator<Particle>*, Particle*>>& forceParticles, unordered_set<ForceGenerator<Particle>*>& forceGens) {
			generateBuoyancySystem(forceParticles, forceGens);
			});
		break;
		// slinky
	case '4':
		launch([this](vector<std::pair<ForceGenerator<Particle>*, Particle*>>& forceParticles, unordered_set<ForceGenerator<Particle>*>& forceGens) {
			generateSlinkySystem(forceParticles, forceGens);
			});
		break;
		// disminuir k de los muelles
	case '7':
		if (springFg != nullptr) {
			springFg->decreaseK();
		}
		break;
		// aumentar k de los muelles
	case '8':
		if (springFg != nullptr) {
			springFg->increaseK();
		}
		break;
		// disminuir masa para la flotacion
	case '9':
		if (forceGenerators[BuoyanceFg] != nullptr) {
			for (auto particle : launcherParticleGen->findParticleByForce(forceGenerators[BuoyanceFg])) {
				particle->disminuirMasa();
			}
		}
		break;
		// aumentar masa para la flotacion
	case '0':
		if (forceGenerators[BuoyanceFg] != nullptr) {
			for (auto particle : launcherParticleGen->findParticleByForce(forceGenerators[BuoyanceFg])) {
				particle->aumentarMasa();
			}
		}
		break;
		// disminuir tam y volumen para la flotacion
	case 'O':
		if (forceGenerators[BuoyanceFg] != nullptr) {
			for (auto particle : launcherParticleGen->findParticleByForce(forceGenerators[BuoyanceFg])) {
				particle->disminuirTam();
			}
		}
		break;
		// aumentar tam y volumen para la flotacion
	case 'P':
		if (forceGenerators[BuoyanceFg] != nullptr) {
			for (auto particle : launcherParticleGen->findParticleByForce(forceGenerators[BuoyanceFg])) {
				particle->aumentarTam();
			}
		}
		break;
		// generadores de particulas con fuerzas
	case 'Z':
		changeActiveGen(AplicarFuerzaGen);
		cambiarFuerzasGen = aplicarFuerzaGen;
		break;
	case 'X':
		selectNextGen(AplicarFuerzaGen, ExplosionGen);
		if (isGenActive(AplicarFuerzaGen)) {
			cambiarFuerzasGen = aplicarFuerzaGen;
		}
		break;
		// activar explosion
	case 'C':
		if (isGenActive(ExplosionGen)) {
			explosionFg->explode();
		}
		break;
		// añadir/quitar gravedad
	case 'T':
		if (isGenActive(AplicarFuerzaGen) || isGenActive(LaunchGen)) {
			this->toggleForce(cambiarFuerzasGen, GravityFg);
		}
		break;
		// añadir/quitar viento
	case 'Y':
		if (isGenActive(AplicarFuerzaGen) || isGenActive(LaunchGen)) {
			this->toggleForce(cambiarFuerzasGen, WindFg);
		}
		break;
		// añadir/quitar torbellino
	case 'U':
		if (isGenActive(AplicarFuerzaGen) || isGenActive(LaunchGen)) {
			this->toggleForce(cambiarFuerzasGen, WhirlWindFg);
		}
		break;
		// generadores de la practica 2
	case 'V':
		changeActiveGen(FuenteGen);
		break;
	case 'B':
		selectNextGen(FuenteGen, NieblaGen);
		break;
		// lanzar firework 1
	case 'N':
		changeActiveGen(Fire1Gen);
		break;
		// lanzar firework circular
	case 'M':
		changeActiveGen(CircleGen);
		break;
		// disminuir la velocida simulada para la manguera gaussiana
	case 'H':
		if (isGenActive(MangueraGaussianaGen)) {
			getParticleGenerator(MangueraGaussianaGen)->decreaseSimulatedV();
		}
		break;
		// aumentar la velocidad simulada para la manguera gaussiana
	case 'J':
		if (isGenActive(MangueraGaussianaGen)) {
			getParticleGenerator(MangueraGaussianaGen)->increaseSimulatedV();
		}
		break;
		// disminuir el ancho de la manguera gaussiana
	case 'K':
		if (isGenActive(MangueraGaussianaGen)) {
			auto stdDevVel = mangueraGaussianGen->getStdDevVel();
			stdDevVel.z -= 0.1;
			if (stdDevVel.z < 0) {
				stdDevVel.z = 0;
			}
			mangueraGaussianGen->setStdDevVel(stdDevVel);
		}
		break;
		// aumentar el ancho de la manguera guassiana
	case 'L':
		if (isGenActive(MangueraGaussianaGen)) {
			auto stdDevVel = mangueraGaussianGen->getStdDevVel();
			stdDevVel.z += 0.1;
			mangueraGaussianGen->setStdDevVel(stdDevVel);
		}
		break;
	}
}