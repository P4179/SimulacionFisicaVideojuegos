#pragma once
#include "RigidBodyGenerator.h"

class UniformRBGenerator : public RigidBodyGenerator {
private:
	Vector3 velWidth;
	Vector3 posWidth;
	uniform_real_distribution<double> dist;

public:
	UniformRBGenerator(RBGens gen, Vector3 meanPos, Vector3 meanVel, double genProbability, int numParticles, ParticleInfo RBfeat,
		Vector3 velWidth, Vector3 posWidth) :
		RigidBodyGenerator(gen, meanPos, meanVel, genProbability, numParticles, RBfeat), velWidth(velWidth), posWidth(posWidth),
		dist(uniform_real_distribution<double>(-1.0, 1.0)) {}

	virtual void generateRB() {
		if (probability()) {
			list<DynamicRigidBody*> list;
			for (int i = 0; i < numParticles; ++i) {
				Vector3 pos = meanPos;
				pos.x += dist(mt) * posWidth.x;
				pos.y += dist(mt) * posWidth.y;
				pos.z += dist(mt) * posWidth.z;

				Vector3 vel = meanVel;
				vel.x += dist(mt) * velWidth.x;
				vel.y += dist(mt) * velWidth.y;
				vel.z += dist(mt) * velWidth.z;

				DynamicRigidBody* rigidBody = createRigidBody(pos, vel);
				list.push_back(rigidBody);
			}
			add(list);
		}
	}
};