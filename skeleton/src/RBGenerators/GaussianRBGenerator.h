#pragma once
#include "RigidBodyGenerator.h"

class GaussianRBGenerator : public RigidBodyGenerator {
private:
	Vector3 standardDesvPos;
	Vector3 standardDesvVel;

	double variation(double standardDesv) {
		if (standardDesv > 0) {
			normal_distribution<double> dist = normal_distribution<double>(0, standardDesv);
			return dist(mt);
		}
		else {
			return 0;
		}
	}

public:
	GaussianRBGenerator(RBGens gen, Vector3 meanPos, Vector3 meanVel, double genProbability, int numParticles, ParticleInfo RBfeat, Vector3 standardDesvPos, Vector3 standardDesvVel) :
		RigidBodyGenerator(gen, meanPos, meanVel, genProbability, numParticles, RBfeat), standardDesvPos(standardDesvPos), standardDesvVel(standardDesvVel) {}

	virtual void generateRB() {
		if (probability()) {
			list<DynamicRigidBody*> list;
			for (int i = 0; i < numParticles; ++i) {
				Vector3 pos = meanPos;
				pos.x += variation(standardDesvPos.x);
				pos.y += variation(standardDesvPos.y);
				pos.z += variation(standardDesvPos.z);

				Vector3 vel = meanVel;
				vel.x += variation(standardDesvVel.x);
				vel.y += variation(standardDesvVel.y);
				vel.z += variation(standardDesvVel.z);

				DynamicRigidBody* rigidBody = createRigidBody(pos, vel);
				list.push_back(rigidBody);
			}
			add(list);
		}
	}

};