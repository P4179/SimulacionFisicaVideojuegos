#pragma once
#include <list>

class Particle;

using namespace std;

class ListParticles {
private:
	int maxParticles;
	std::list<Particle*> listP;

	void removeExcess();

public:
	ListParticles(int maxParticles);

	virtual ~ListParticles();

	void add(list<Particle*> newListP);

	void kill();

	void refresh();

	void integrate(double t);
};