#pragma once
#include "UniformParticleGenerator.h"

using namespace std;

class ForceParticleGenerator : public UniformParticleGenerator {
private:
	vector<ForceGenerator*> forceGens;

public:
	ForceParticleGenerator(string name, Vector3 meanPos, Vector3 meanVel, ParticleInfo info, double genProbability, int numParticles, Vector3 velWidth, Vector3 posWidth, const vector<ForceGenerator*>& forceGens) :
		UniformParticleGenerator(name, meanPos, meanVel, info, genProbability, numParticles, velWidth, posWidth), forceGens(forceGens) {}

	virtual Particle* createParticle(Vector3 pos, Vector3 vel) {
		return new Particle(pos, vel, _info.invMasa, _info.damping, _info.lifeTime, _info.vSimulada, _info.radius, _info.color);
	}

	virtual void update(ListParticles* particles) {
		particles->add(generateParticles(), forceGens);
	}
};