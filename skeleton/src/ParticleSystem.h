#pragma once
#include <list>
#include <unordered_map>
#include "./Generators/UniformParticleGenerator.h"
#include "./Generators/GaussianParticleGenerator.h"
#include "./Generators/FireworkGenerator.h"
#include "./Generators/ParticleGenerator.h"
#include "./Generators/CircleGenerator.h"
#include "./ListParticles.h"

using namespace std;

// el último valor sirve para saber cuantos generadores hay en total
enum Generators {
	Fuente, Manguera, LLuvia, MangueraGaussiana, Niebla,
	Fire1, Fire2, Fire3, Fire4, Circle, Max
};

const vector<string> generatorsNames {
	"Fuente", "Manguera", "LLuvia", "MangueraGaussiana", "Niebla",
		"Fire1", "Fire2", "Fire3", "Fire4", "Circle"
};

class ParticleSystem {
private:
	const float DAMPING = 0.8;

	ListParticles* particles;
	vector<ParticleGenerator*> particle_generators;
	unordered_map<string, ParticleGenerator*> particleGenFindByName;
	Vector3 gravity;
	Generators selectedGen;
	GaussianParticleGenerator* mangueraGaussiana;

	// generador normal
	template<typename T, typename ...Ts>
	T* addParticleGenerator(Generators gen, Ts&& ...args) {
		T* generator = new T(generatorsNames[gen], forward<Ts>(args)...);
		particle_generators[gen] = generator;
		particleGenFindByName.insert({ generatorsNames[gen], generator });
		return generator;
	}

public:
	ParticleSystem(Vector3 gravity = Vector3(0, -10, 0));

	void generateFireworkSystem();

	virtual ~ParticleSystem();

	void integrate(double t);

	void keyPressed(int __cdecl key);

	inline ParticleGenerator* getParticleGenerator(string name) const {
		auto it = particleGenFindByName.find(name);
		if (it == particleGenFindByName.end()) {
			throw exception("It does not exist a particle generator with that name");
		}
		else {
			return (*it).second;
		}
	}

	inline ParticleGenerator* getParticleGenerator(Generators gen) const {
		return particle_generators[gen];
	}
};