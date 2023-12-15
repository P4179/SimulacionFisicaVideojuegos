#pragma once
#include "../RigidBodies/SphereDynamicRB.h"
#include "../RigidBodies/BoxDynamicRB.h"
#include "../RBStructure/RigidBodySystem.h"
#include <list>
#include <random>
#include "../particle_info.h"

using namespace std;

class RigidBodyGenerator {
private:
	RigidBodySystem* system;
	RBGens gen;
	double genProbability;	// [0, 1]
	ParticleInfo RBfeat;
	uniform_real_distribution<double> u;

protected:
	Vector3 meanPos;
	Vector3 meanVel;
	int numParticles;
	std::mt19937 mt;

	DynamicRigidBody* createRigidBody(Vector3 pos, Vector3 vel);

	RigidBodyGenerator(RBGens gen, Vector3 meanPos, Vector3 meanVel, double genProbability, int numParticles, ParticleInfo RBfeat);

	inline void add(list<DynamicRigidBody*> list) {
		system->add(list);
	}

	inline bool probability() {
		double prob = u(mt);
		return genProbability > prob;
	}

public:
	virtual void generateRB() = 0;

	inline string getName() const {
		return RB_GENS_NAMES.at(gen);
	}

	inline RBGens getType() const {
		return gen;
	}
};