#include "RigidBodySystem.h"
#include "../Generators/UniformParticleGenerator.h"
#include "../Generators/GaussianParticleGenerator.h"
#include "../checkML.h"

RigidBodySystem::RigidBodySystem(PxPhysics* gPhysics, PxScene* gScene) : gPhysics(gPhysics), gScene(gScene),
RBGenerators(MAX_GENS), RBfGenerators(MAX_FGENS), floor(nullptr), rigidBodies() {

	registry = new ParticleForceRegistry<Particle>();

	// se crean todas las fuerzas
	explosionFg = new ExplosionForceGenerator<Particle>(RB_F_GENS_NAMES.at(EXPLOSION_FG), Vector3(0, 50, 0), 3000, 20, 15, -1, true);
	RBfGenerators.all[EXPLOSION_FG] = explosionFg;
}

void RigidBodySystem::addGenerator(ParticleGenerator* gen, RBGens name, bool enable) {
	RBGenerators.all[name] = gen;
	if (enable) {
		RBGenerators.actives.insert(gen);
	}
}

void RigidBodySystem::refresh() {
	rigidBodies.erase(std::remove_if(rigidBodies.begin(), rigidBodies.end(),
		[this](Particle* rigidBody) {
			if (rigidBody->isAlive()) {
				return false;
			}
			else {
				registry->deleteParticleRegistry(rigidBody);

				delete rigidBody;
				return true;
			}
		}), rigidBodies.end());
}

void RigidBodySystem::toggleForce(RBfGens fGen) {
	auto fg = RBfGenerators.all[fGen];
	auto it = RBfGenerators.actives.find(fg);
	// no esta
	if (it == RBfGenerators.actives.end()) {
		cout << fg->getName() << " incluido" << "\n";
		RBfGenerators.actives.insert(fg);
		// se a�ade la fuerza a las particulas existentes
		for (auto rigidBody : rigidBodies) {
			registry->addRegistry(fg, rigidBody);
		}
	}
	// si esta
	else {
		cout << fg->getName() << " eliminado" << "\n";
		registry->deleteForceRegistry(fg);
		RBfGenerators.actives.erase(it);
	}
}

void RigidBodySystem::toggleGen(RBGens gen) {
	auto generator = RBGenerators.all[gen];
	auto it = RBGenerators.actives.find(generator);
	// no esta
	if (it == RBGenerators.actives.end()) {
		cout << generator->getName() << " incluido" << "\n";
		RBGenerators.actives.insert(generator);
	}
	// si esta
	else {
		cout << generator->getName() << " eliminado" << "\n";
		RBGenerators.actives.erase(generator);
	}
}

RigidBodySystem::~RigidBodySystem() {
	delete floor;

	for (auto& rigidBody : rigidBodies) {
		delete rigidBody;
	}

	for (auto& gen : RBGenerators.all) {
		delete gen;
	}

	for (auto& fGen : RBfGenerators.all) {
		delete fGen;
	}

	delete registry;
}

void RigidBodySystem::onCollision(physx::PxActor* actor1, physx::PxActor* actor2) {
	/*
	if (floor->getActor() == actor1) {
		cout << "Actor1 is a floor\n";
	}
	else if (floor->getActor() == actor2) {
		cout << "Actor2 is a floor\n";
	}
	*/
}

void RigidBodySystem::create() {
	floor = new BoxStaticRB(gPhysics, gScene, Vector3(0, 0, 0), Vector4(0.118, 0.569, 0.22, 1), Vector3(100, 1, 100));

	ParticleInfo info;
	info.damping = DAMPING;

	// se crean todos los generadores
	info.color = Vector4(0.204, 0.8, 0.82, 1);
	info.lifeTime = 8;

	info.geometry = physx::PxGeometryType::eSPHERE;
	info.sphere_data.radius = 1.2;

	info.rigidBody_data.massDef = InertiaTensor;
	info.rigidBody_data.inertiaTensor_data.massDistribution = Vector3(1.0, 2.0, 3.0);
	// coef de rozamiento estatico --> resistencia al inicio del movimiento entre dos en reposo relativo (cuando estan parados)
	// coef dinamico --> resistencia entre dos objetos en movimiento relativo (cuando se estan moviendo)
	// coef rebote
	// Sueles estar entre [0, 1] si se quiere conseguir una simulacion mas coherente con las leyes de la fisica
	info.rigidBody_data.material = gPhysics->createMaterial(0.2, 0.6, 2);

	// pos original, velocidad original, info particula, probabilidad, numero particulas, variacion velocidad, variacion posicion
	addGenerator(new UniformParticleGenerator(RB_GENS_NAMES.at(UNIFORM_GEN), Vector3(0, 150, 0), Vector3(0, -100, 0), info, 0.2, 2, Vector3(0, 10, 0), Vector3(80, 5, 80), {}, gPhysics, gScene),
		UNIFORM_GEN, true);


	info.color = Vector4(0.337, 0.192, 0.8, 1);
	info.lifeTime = 10;

	info.geometry = physx::PxGeometryType::eBOX;
	info.box_data.size = Vector3(0.6, 0.3, 0.3);

	info.rigidBody_data.massDef = Density;
	info.rigidBody_data.density_data.density = 0.1;
	// coef de rozamiento estatico --> resistencia al inicio del movimiento entre dos en reposo relativo (cuando estan parados)
	// coef dinamico --> resistencia entre dos objetos en movimiento relativo (cuando se estan moviendo)
	// coef rebote
	// Sueles estar entre [0, 1] si se quiere conseguir una simulacion mas coherente con las leyes de la fisica
	info.rigidBody_data.material = gPhysics->createMaterial(0.2, 0.3, 2);

	// pos original, velocidad original, info particula, probabilidad, numero particulas, variacion velocidad, variacion posicion
	addGenerator(new GaussianParticleGenerator(RB_GENS_NAMES.at(GAUSSIAN_GEN), Vector3(0, 20, 0), Vector3(0, 70, 0), info, 0.2, 1, Vector3(5, 0, 5), Vector3(2, 0, 2)),
		GAUSSIAN_GEN, true);
}

void RigidBodySystem::integrate(double t) {
	registry->updateTime(t);
	registry->updateForces(t);

	for (auto rigidBody : rigidBodies) {
		rigidBody->integrate(t);
	}

	refresh();

	if (rigidBodies.size() < MAX) {
		for (auto gen : RBGenerators.actives) {
			add(gen->generateParticles());
		}
	}
}

void RigidBodySystem::keyPressed(int __cdecl key) {
	switch (key) {
	// toggle gen particulas 1
	case 'Z':
		toggleGen(UNIFORM_GEN);
		break;
	// toggle gen particulas 2
	case 'X':
		toggleGen(GAUSSIAN_GEN);
		break;
	// toggle explosion
	case 'C':
		toggleForce(EXPLOSION_FG);
		break;
	// habilitar/deshabilitar explosion
	case 'V':
		if (isForceActivated(EXPLOSION_FG)) {
			explosionFg->explode();
		}
		break;
	}
}

void RigidBodySystem::add(list<Particle*> list) {
	for (auto rigidBody : list) {
		for (auto fg : RBfGenerators.actives) {
			registry->addRegistry(fg, rigidBody);
		}
		rigidBodies.push_back(rigidBody);
	}
}