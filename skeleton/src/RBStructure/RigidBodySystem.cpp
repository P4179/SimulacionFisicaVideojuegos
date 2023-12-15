#include "RigidBodySystem.h"
#include "../RBGenerators/RigidBodyGenerator.h"
#include "../RBGenerators/UniformRBGenerator.h"
#include "../RBGenerators/GaussianRBGenerator.h"

RigidBodySystem::RigidBodySystem(PxPhysics* gPhysics, PxScene* gScene) : gPhysics(gPhysics), gScene(gScene),
RBGenerators(MAX_GENS), RBfGenerators(MAX_FGENS) {

	floor = new BoxStaticRB(gPhysics, gScene, Vector3(0, 0, 0), Vector4(1, 0, 0, 1), Vector3(200, 10, 200));
	list<DynamicRigidBody*> list;
	list.push_back(new SphereDynamicRB(gPhysics, gScene, Vector3(-20, 100, 0), Vector3(0, -1, 0), Vector3(0, 0, 0), DAMPING, 0.15, Vector4(1, 1, 0, 1), 3));
	list.push_back(new SphereDynamicRB(gPhysics, gScene, Vector3(20, 100, 0), Vector3(0, -1, 0), Vector3(0, 0, 0), DAMPING, 0.15, Vector4(1, 0, 1, 1), 3));
	add(list);
	registry = new ParticleForceRegistry<DynamicRigidBody>();

	// se crean todos los generadores

	// se crean todas las fuerzas
	explosionFg = new ExplosionForceGenerator<DynamicRigidBody>(RB_F_GENS_NAMES.at(EXPLOSION_FG), Vector3(0, 50, 0), 20, 3000, 15, -1, true);
	RBfGenerators.all[EXPLOSION_FG] = explosionFg;
}

void RigidBodySystem::addGenerator(RigidBodyGenerator* gen, bool enable) {
	RBGenerators.all[gen->getType()] = gen;
	if (enable) {
		RBGenerators.actives.insert(gen);
	}
}

void RigidBodySystem::refresh() {
	rigidBodies.erase(std::remove_if(rigidBodies.begin(), rigidBodies.end(),
		[this](DynamicRigidBody* rigidBody) {
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
		// se añade la fuerza a las particulas existentes
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

void RigidBodySystem::integrate(double t) {

	registry->updateTime(t);
	registry->updateForces(t);

	for (auto rigidBody : rigidBodies) {
		rigidBody->updateLifeTime(t);
	}

	refresh();

	if (rigidBodies.size() < MAX) {
		for (auto gen : RBGenerators.actives) {
			gen->generateRB();
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
			explosionFg->enableExplosion();
		}
		break;
	}
}

void RigidBodySystem::add(list<DynamicRigidBody*> list) {
	for (auto rigidBody : list) {
		for (auto fg : RBfGenerators.actives) {
			registry->addRegistry(fg, rigidBody);
		}
		rigidBodies.push_back(rigidBody);
	}
}