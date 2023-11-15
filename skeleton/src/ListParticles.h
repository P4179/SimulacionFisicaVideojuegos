#pragma once
#include <list>

class Particle;
class ParticleForceRegistry;
class ForceGenerator;

using namespace std;

class ListParticles {
private:
	int maxParticles;
	std::list<Particle*> listP;
	ParticleForceRegistry* registry;

	void removeExcess();

public:
	ListParticles(int maxParticles, ParticleForceRegistry* registry);

	virtual ~ListParticles();

	void add(list<Particle*> newListP, const vector<ForceGenerator*>& forceGens);

	void add(list<Particle*> newListP);

	void kill();

	void refresh();

	void integrate(double t);
};