#pragma once
#include "../RigidBodies/SphereDynamicRB.h"
#include "../RBStructure/RigidBodySystem.h"
#include <list>
#include <random>

using namespace std;

enum Shapes { Box, Sphere };
enum MassDefinition { Density, InertiaTensor };

struct DynamicRBFeatures {
	Vector4 color;
	float lifeTime;

	Shapes shape;
	struct {
		Vector3 size;
	} box_data;
	struct {
		float radius;
	} sphere_data;

	MassDefinition massDef;
	struct {
		// kg/m^3
		float density;
	} density_data;
	MassDefinition inertiaTensor;
	struct {
		Vector3 massDistribution;
	} inertiaTensor_data;
};

class RigidBodyGenerator {
private:
	RigidBodySystem* system;
	RBGens gen;
	double genProbability;	// [0, 1]
	DynamicRBFeatures RBfeat;
	float damping;
	uniform_real_distribution<double> u;

protected:
	Vector3 meanPos;
	Vector3 meanVel;
	int numParticles;
	std::mt19937 mt;

	DynamicRigidBody* createRigidBody(Vector3 pos, Vector3 vel);

	RigidBodyGenerator(RBGens gen, Vector3 meanPos, Vector3 meanVel, double genProbability, int numParticles, DynamicRBFeatures RBfeat, float damping);

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