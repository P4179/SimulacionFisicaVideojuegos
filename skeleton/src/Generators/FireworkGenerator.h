#pragma once
#include <vector>
#include "GaussianParticleGenerator.h"

class Firework;

using namespace std;

class FireworkGenerator : public GaussianParticleGenerator {
private:
	static vector<FireworkGenerator*> fireworks;

protected:
	virtual Particle* createParticle(Vector3 vel, Vector3 pos);

public:
	FireworkGenerator(string name, Vector3 mean_pos, Vector3 mean_vel, ParticleInfo info, int num_particles, Vector3 stdDevVel, Vector3 stdDevPos);

	// el fireworkGenerator contiene los generadores que se pueden utilizar para crear m�s part�culas
	// cuando las part�culas que ha creado mueran
	static void addFireworkGen(FireworkGenerator* firework) {
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