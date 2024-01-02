#include "Scene.h"

void Scene::createTexts() {
	texts[T_COUNTER] = new DisplayText("CONTADOR: " + to_string(MAX_COUNTER - counter), { 0.239f, 0.239f, 0.239f, 1.0f }, { 10,480 }, GLUT_BITMAP_HELVETICA_18);
	RegisterDisplayText(texts[T_COUNTER]);

	texts[T_WIN] = new DisplayText("WIN", { 0.086f, 0.639f, 0.114f, 1.0f }, { 240,250 }, GLUT_BITMAP_TIMES_ROMAN_24);

	texts[T_GAMEOVER] = new DisplayText("GAMEOVER", { 1.0f, 0.0f, 0.0f, 1.0f }, { 210,250 }, GLUT_BITMAP_TIMES_ROMAN_24);
}

void Scene::createPlatform() {
	// principio (130 entre cada una)
	platforms.push_back({ new BoxStaticRB(gPhysics, gScene, Vector3(0, 0, 0), Vector4(0.851, 0.69, 0.427, 1), PLATFORM_SIZE, gPhysics->createMaterial(0.697, 0.781, 0)),
		false });

	platforms.push_back({ new BoxStaticRB(gPhysics, gScene, Vector3(0, 10, -130), Vector4(0.706, 0.851, 0.427, 1), PLATFORM_SIZE, gPhysics->createMaterial(0.43, 0.865 , 0)),
		false });

	// izquierda (100 a la primera, 140 entre cada una)
	platforms.push_back({ new BoxStaticRB(gPhysics, gScene, Vector3(-120, 10, -230), Vector4(0.427, 0.851, 0.675, 1), PLATFORM_SIZE, gPhysics->createMaterial(0.571, 0.691, 0)),
		false });

	platforms.push_back({ new BoxStaticRB(gPhysics, gScene, Vector3(-120, 5, -365), Vector4(0.706, 0.427, 0.851, 1), PLATFORM_SIZE, gPhysics->createMaterial(0.484, 0.393, 0)),
		false });

	platforms.push_back({ new BoxStaticRB(gPhysics, gScene, Vector3(-120, 10, -505), Vector4(0.878, 0.388, 0.741, 1), PLATFORM_SIZE, gPhysics->createMaterial(0.5, 0.234, 0)),
		false });

	// derecha (100 a la primera, 140 entre cada una)
	platforms.push_back({ new BoxStaticRB(gPhysics, gScene, Vector3(120, 10, -230), Vector4(0.878, 0.388, 0.486, 1), PLATFORM_SIZE, gPhysics->createMaterial(0.606, 0.637, 0)),
		false });

	platforms.push_back({ new BoxStaticRB(gPhysics, gScene, Vector3(120, 5, -365), Vector4(0.388, 0.435, 0.878, 1), PLATFORM_SIZE, gPhysics->createMaterial(0.5, 0.777, 0)),
		false });

	platforms.push_back({ new BoxStaticRB(gPhysics, gScene, Vector3(120, 30, -505), Vector4(0.388, 0.878, 0.431, 1), PLATFORM_SIZE, gPhysics->createMaterial(0.426, 0.876, 0)),
		false });

	// final (105 la primera, 130 entre cada una)
	platforms.push_back({ new BoxStaticRB(gPhysics, gScene, Vector3(0, 15, -610), Vector4(0.929, 0.906, 0.235, 1), PLATFORM_SIZE, gPhysics->createMaterial(0.472, 0.728, 0)),
		false });

	platforms.push_back({ new BoxStaticRB(gPhysics, gScene, Vector3(0, 15, -740), Vector4(0.929, 0.675, 0.235, 1), PLATFORM_SIZE, gPhysics->createMaterial(0.67, 0.54, 0)),
		false });
}

void Scene::createForces() {
	BuoyancyForceGenerator::LiquidVisual visual;
	visual.x = 0;
	visual.z = HALF_AREA;
	visual.size = 800;
	visual.color = Vector4(0.31, 0.769, 1, 1);
	forces[FG_BUOYANCY] = new BuoyancyForceGenerator("BuoyancyFg", -200, 100, WATER_DENSITY, visual);

	forces[FG_WIND_LEFT] = new WindForceGenerator("WindLeftFg", { Vector3(-10, 0, 0), Vector3(-15, 0, 0), Vector3(-20, 0, 0) }, Vector3(-120, -12, -230), PLATFORM_SIZE.x / 2 * 1.3, -1, true, DEBUG);

	forces[FG_WIND_UP] = new WindForceGenerator("WindUpFg", { Vector3(0, 70, -20), Vector3(0,75, -15) }, Vector3(180, 0, -465), 60, -1, true, DEBUG);

	explosionFg = new ExplosionForceGenerator<Particle>("ExplosionFg", Vector3(-120, 10, -505), PLATFORM_SIZE.x / 2 * 1.5, 2800000, 12, -1, DEBUG, true, 100);
	forces[FG_EXPLOSION] = explosionFg;

	forces[FG_GRAVITY] = new GravityForceGenerator("GravityFg", GRAVITY);
}

void Scene::createPlayer() {
	character = new Character(gPhysics, gScene, Vector3(0, 60, 0), 0.4, 2, 2.5);
	registry->addRegistry(forces[FG_BUOYANCY], character);
	registry->addRegistry(forces[FG_WIND_LEFT], character);
	registry->addRegistry(forces[FG_WIND_UP], character);
	registry->addRegistry(forces[FG_EXPLOSION], character);
}

void Scene::createGenerators() {
	unordered_set<ForceGenerator<Particle>*> currentForces = { forces[FG_GRAVITY], forces[FG_WIND_LEFT], forces[FG_WIND_UP], forces[FG_EXPLOSION] };

	// RENDER PARTICLE
	ParticleInfo info;
	info.damping = DAMPING;

	info.geometry = physx::PxGeometryType::eSPHERE;
	info.renderParticle_data.invMasa = 0.2;
	info.color = Vector4(0.059, 0.231, 0.404, 1);
	info.lifeTime = 14;
	info.sphere_data.radius = 1.4;
	info.renderParticle_data.vSimulada = 100;
	// pos original, velocidad original, info particula, probabilidad, numero particulas, variacion velocidad, variacion posicion
	generators.push_back(new UniformParticleGenerator("FontainGen", Vector3(0, -100, HALF_AREA), Vector3(0, 1, 0), info, 0.35, 2, Vector3(0.5, 0, 0.5), Vector3(3, 0, 3), currentForces));

	info.geometry = physx::PxGeometryType::eSPHERE;
	info.renderParticle_data.invMasa = 0.5;
	info.color = Vector4(0.612, 0.612, 0.612, 1);
	info.lifeTime = 8;
	info.sphere_data.radius = 0.6;
	info.renderParticle_data.vSimulada = 90.0;
	generators.push_back(new UniformParticleGenerator("RainGen", Vector3(0, 200, HALF_AREA - 100), Vector3(0, -1, 0), info, 0.5, 3, Vector3(0, 0, 0), Vector3(250, 5, 350), currentForces));

	createFireworks();

	// RIGID BODY
	currentForces.erase(forces[FG_GRAVITY]);

	info.geometry = physx::PxGeometryType::eBOX;
	info.rigidBody_data.massDef = InertiaTensor;
	info.rigidBody_data.inertiaTensor_data.massDistribution = Vector3(9, 10, 11);
	info.rigidBody_data.material = gPhysics->createMaterial(0.8, 0.9, 0);
	info.color = Vector4(0.412, 0.412, 0.412, 1);
	info.lifeTime = 3;
	info.box_data.size = Vector3(0.6, 0.6, 0.6);
	generators.push_back(new UniformParticleGenerator("BlockGen", Vector3(0, 200, -615), Vector3(0, -200, 0), info, 1.3, 2, Vector3(0, 0, 0), Vector3(PLATFORM_SIZE.x / 2 + 55, 0, PLATFORM_SIZE.z / 2 + 55), currentForces, gPhysics, gScene));
}

void Scene::createFireworks() {
	auto currentForces = { forces[FG_GRAVITY] };

	// el firework siempre es una particula de forma esferica, por lo tanto,
	// no hace falta aportar tanta info
	ParticleInfo info;
	info.renderParticle_data.invMasa = 0.2;
	info.damping = DAMPING;

	// LANZADOR
	info.color = Vector4(0.922, 0.906, 0.18, 1);
	info.lifeTime = 1;
	info.sphere_data.radius = 1.3;
	info.renderParticle_data.vSimulada = 30;
	try {
		fireworkLauncher = new CircleGenerator("CircleFireGen", Vector3(), info, 20, Vector3(0, 0, 0), Vector3(0, 1, 0), Vector3(1, 0, 0), currentForces);
		addFireworkGen(false, fireworkLauncher);
	}
	catch (exception e) {
		cout << "It could not be created a circle generator" << "\n";
	}

	// SE PROPAGAN
	info.color = Vector4(0.208, 1, 0.082, 1);
	info.lifeTime = 1.3;
	info.sphere_data.radius = 0.8;
	info.renderParticle_data.vSimulada = 30;
	// la posicion no hace falta darla xq se va a decidir en el momento en funcion de la pos del jugador
	// pos media, velocidad media, info particula, numero particulas, variacion velocidad, variacion posicion
	addFireworkGen(true, new FireworkGenerator("FireworkGen1", Vector3(), Vector3(0.2, 0.3, 0.2), info, 2, Vector3(0.8, 0.2, 0.8), Vector3(0, 0, 0), currentForces));

	info.color = Vector4(0.984, 0.082, 1, 1);
	info.lifeTime = 1.2;
	info.sphere_data.radius = 0.7;
	info.renderParticle_data.vSimulada = 35;
	addFireworkGen(true, new FireworkGenerator("FireworkGen2", Vector3(), Vector3(0.25, 0.1, 0), info, 3, Vector3(0.2, 0.15, 0.2), Vector3(0, 0, 0), currentForces));

	info.color = Vector4(1, 0.22, 0.22, 1);
	info.lifeTime = 1.2;
	info.sphere_data.radius = 0.7;
	info.renderParticle_data.vSimulada = 35;
	addFireworkGen(true, new FireworkGenerator("FireworkGen3", Vector3(), Vector3(-0.25, 0.1, 0), info, 3, Vector3(0.2, 0.15, 0.2), Vector3(0, 0, 0), currentForces));
}

Scene::Scene(physx::PxPhysics* gPhysics, physx::PxScene* gScene) : counter(0), texts(T_MAX, nullptr), platforms(), generators(), forces(FG_MAX, nullptr),
buoyancyFg(nullptr), character(nullptr), gPhysics(gPhysics), gScene(gScene), camera(GetCamera()), elapsedTimeEnd(0), gameover(false),
explosionFg(nullptr), win(false), fireworkLauncher(nullptr), elapsedTimeExplosion(0) {

	camera->setDir(Vector3(0, 0, -1));

	createTexts();

	registry = new ParticleForceRegistry<Particle>();
	listParticles = new ListParticles(MAX_PARTICLES, registry);

	createPlatform();

	createForces();

	createGenerators();
}

Scene::~Scene() {
	for (auto& text : texts) {
		text->release();
	}
	for (auto& platform : platforms) {
		delete platform.first;
	}
	for (auto& gen : generators) {
		delete gen;
	}
	for (auto& force : forces) {
		delete force;
	}
	delete listParticles;
	delete character;
	delete collisionManager;
	// se tiene que borrar despues de la lista de particulas
	delete registry;
}

void Scene::processContactPlatform(StaticRigidBody* plat) {
	auto it = std::find_if(platforms.begin(), platforms.end(), [plat](const std::pair<BoxStaticRB*, bool>& platform) {
		return plat == platform.first;
		});

	if (it != platforms.end()) {
		if (!(*it).second) {
			(*it).second = true;
			increaseCounter();
			if (counter >= MAX_COUNTER) {
				// SE GANA
				hasWinned();
			}
		}
	}
}

void Scene::create() {
	createPlayer();

	vector<BoxStaticRB*> aux(platforms.size());
	int i = 0;
	for (auto platform : platforms) {
		aux[i] = platform.first;
		++i;
	}
	collisionManager = new CollisionManager(character, aux);
}

void Scene::integrate(double t) {
	for (auto gen : generators) {
		gen->update(listParticles);
	}

	collisionManager->integrate(t);

	registry->updateTime(t);

	listParticles->integrate(t);

	listParticles->refresh();

	character->integrate(t);

	// EXPLOSION
	if (elapsedTimeExplosion += t) {
		if (elapsedTimeExplosion > EXPLOSION_TIMER) {
			explosionFg->explode();
			elapsedTimeExplosion = 0;
		}
	}

	// FIN DE JUEGO
	if (character->getImmersion() == Particle::Floating || character->getImmersion() == Particle::Full) {
		if (!win) {
			// SE PIERDE
			hasLost();
		}
	}

	if (win || gameover) {
		elapsedTimeEnd += t;
		if (elapsedTimeEnd > RESET_TIMER) {
			if (win) {
				DeregisterDisplayText(texts[T_WIN]);
			}
			else if (gameover) {
				DeregisterDisplayText(texts[T_GAMEOVER]);
			}
			reset();
		}
	}
}