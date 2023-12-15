#pragma once

#include <PxPhysicsAPI.h>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include "../Singleton.h"
#include "../RigidBodies/BoxStaticRB.h"
#include "../RigidBodies/SphereDynamicRB.h"
#include "../ForceGenerators/ForceGenerator.h"
#include "../ForceGenerators/ExplosionForceGenerator.h"
#include "../ParticleForceRegistry.h"

class ParticleGenerator;

using namespace physx;
using namespace std;

enum RBGens { UNIFORM_GEN, GAUSSIAN_GEN, MAX_GENS };

const unordered_map<RBGens, string> RB_GENS_NAMES{
	{UNIFORM_GEN, "UNIFORM"},
	{GAUSSIAN_GEN, "GAUSSIAN_GEN"}
};

enum RBfGens { EXPLOSION_FG, MAX_FGENS };

const unordered_map<RBfGens, string> RB_F_GENS_NAMES{
	{EXPLOSION_FG, "EXPLOSION_F_GEN"},
};

template<typename T>
struct GensSystem {
public:
	vector<T*> all;
	unordered_set<T*> actives;

	GensSystem() {}

	GensSystem(int max) : all(max), actives() {}
};

class RigidBodySystem : public Singleton<RigidBodySystem> {
	// permitir que la clase singleton pueda acceder a los miembros privados de esta clase
	friend Singleton<RigidBodySystem>;

private:
	// expresa la disminucion de la velocidad debido a las fuerzas de resistencia
	const float DAMPING = 0.8;
	// numero maximo de rigidbodies que puede haber en escena
	const int MAX = 200;

	// motor de fisicas
	PxPhysics* gPhysics;
	// escena, donde estan los diferentes elementos/actores
	PxScene* gScene;

	StaticRigidBody* floor;

	list<Particle*> rigidBodies;

	GensSystem<ParticleGenerator> RBGenerators;
	GensSystem<ForceGenerator<Particle>> RBfGenerators;

	ParticleForceRegistry<Particle>* registry;

	ExplosionForceGenerator<Particle>* explosionFg;

	RigidBodySystem() {}

	RigidBodySystem(PxPhysics* gPhysics, PxScene* gScene);

	void addGenerator(ParticleGenerator* gen, RBGens name, bool enable);

	void refresh();

	void toggleForce(RBfGens fGen);

	void toggleGen(RBGens gen);

	bool isForceActivated(RBfGens fGen) {
		auto fg = RBfGenerators.all[fGen];
		return RBfGenerators.actives.find(fg) != RBfGenerators.actives.end();
	}

public:
	virtual ~RigidBodySystem();

	void create();

	void onCollision(physx::PxActor* actor1, physx::PxActor* actor2);

	void integrate(double t);

	void keyPressed(int __cdecl key);

	// fuerzas globales
	void add(list<Particle*> list);

	inline PxPhysics* getPhysics() const {
		return gPhysics;
	}

	inline PxScene* getScene() const {
		return gScene;
	}
};