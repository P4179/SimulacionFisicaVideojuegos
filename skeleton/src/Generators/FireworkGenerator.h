#pragma once
#include <vector>
#include "GaussianParticleGenerator.h"

class Firework;

using namespace std;

class FireworkGenerator : public GaussianParticleGenerator {
private:
	static const float EXPLOSION_PROBABILITY;
	static vector<FireworkGenerator*> fireworks;

protected:
	virtual Particle* createParticle(Vector3 pos, Vector3 vel);

public:
	FireworkGenerator(string name, Vector3 mean_pos, Vector3 mean_vel, ParticleInfo info, int num_particles, Vector3 stdDevVel, Vector3 stdDevPos, const unordered_set<ForceGenerator<Particle>*>& forceGens = unordered_set<ForceGenerator<Particle>*>());

	// el fireworkGenerator contiene los generadores que se pueden utilizar para crear más partículas
	// cuando las partículas que ha creado mueran
	static void addFireworkGen(FireworkGenerator* firework) {
		firework->setProbability(EXPLOSION_PROBABILITY);
		fireworks.push_back(firework);
	}

	static FireworkGenerator* getGenerator(int index) {
		if (index >= fireworks.size()) {
			throw exception("There is not any firework generator associated with that index");
		}
		return fireworks[index];
	}

	static FireworkGenerator* getRndGenerator() {
		int random = rand() % fireworks.size();	// numero entero [0, fireworks.size())
		try {
			getGenerator(random);
		}
		catch (exception e) {
			// se propaga la excepcion
			throw exception(e.what());
		}
	}

	// no sucede nada cuando se produce el update
	virtual void update(ListParticles* particles) {}
};