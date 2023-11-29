#pragma once
#include <list>
#include <unordered_map>
#include <unordered_set>
#include "./Generators/UniformParticleGenerator.h"
#include "./Generators/GaussianParticleGenerator.h"
#include "./Generators/FireworkGenerator.h"
#include "./Generators/ParticleGenerator.h"
#include "./Generators/CircleGenerator.h"
#include "./ListParticles.h"
#include "./ForceGenerators/GravityForceGenerator.h"
#include "ParticleForceRegistry.h"
#include "./Generators/ForceParticleGenerator.h"
#include "./ForceGenerators/WindForceGenerator.h"
#include "./ForceGenerators/WhirlwindForceGenerator.h"
#include "./ForceGenerators/ExplosionForceGenerator.h"

using namespace std;

// el último valor sirve para saber cuantos generadores hay en total
enum Generators {
	// generadores que se ejecutan cada vez
	Fuente, Manguera, LLuvia, MangueraGaussiana, Niebla,
	FuerzaDefecto, Viento1, Viento2, Torbellino, Explosion,
	// generadores que se lanzan al principio o solo sirven para propagar
	Fire1, Fire2, Fire3, Fire4, Circle, MAX, NONE
};

enum ForceGens {
	GravityGen, WindGen, WhirlWindGen, ExplosionGen, MAX_FORCES
};

const unordered_map<Generators, string> generatorsNames{
	{Fuente, "Fuente"},
	{Manguera, "Manguera"},
	{LLuvia, "LLuvia"},
	{MangueraGaussiana, "MangueraGaussiana"},
	{Niebla, "Niebla"},
	{FuerzaDefecto,"FuerzaDefecto"},
	{Viento1, "Viento1"},
	{Viento2, "Viento2"},
	{Torbellino, "Torbellino"},
	{Explosion, "Explosion"},
	{Fire1, "Fire1"},
	{Fire2,"Fire2"},
	{Fire3,"Fire3"},
	{Fire4,"Fire4"},
	{Circle,"Circle"}
};

class ParticleSystem {
private:
	const float DAMPING = 0.8;

	ListParticles* particles;
	// se utiliza para actualizar los generadores y para obtener cada generador
	vector<std::pair<ParticleGenerator*, bool>> particle_generators;
	unordered_map<string, ParticleGenerator*> particleGenFindByName;
	Vector3 gravity;
	GaussianParticleGenerator* mangueraGaussiana;
	ForceParticleGenerator* fuerzaDefGenParticula;
	ExplosionForceGenerator* explosionGen;
	Generators selectedGen;

	vector<ForceGenerator*> forceGenerators;
	ParticleForceRegistry* registry;

	// generador normal
	template<typename T, typename ...Ts>
	T* addParticleGenerator(Generators gen, bool enable, Ts&& ...args) {
		// at funciona igual que [], pero con verificador de limites
		T* generator = new T(generatorsNames.at(gen), forward<Ts>(args)...);
		particle_generators[gen] = { generator, enable };
		if (enable) {
			selectedGen = gen;
		}
		particleGenFindByName[generatorsNames.at(gen)] = generator;
		return generator;
	}

	template<typename T, typename ...Ts>
	FireworkGenerator* addFireworkGenerator(Generators gen, bool enable, bool included, Ts&& ...args) {
		// en los fireworks la segunda componente se utiliza para saber si se ha iniciado con ese o no
		FireworkGenerator* fireworkGenerator = addParticleGenerator<T>(gen, enable, forward<Ts>(args)...);
		if (included) {
			FireworkGenerator::addFireworkGen(fireworkGenerator);
		}
		if (enable) {
			selectedGen = gen;
			fireworkGenerator->init(particles);
		}
		return fireworkGenerator;
	}

	inline void disableAllGenerators() {
		for (auto& generator : particle_generators) {
			generator.second = false;
		}
	}

	inline void changeActiveGen(Generators gen) {
		// se deregistrar particulas y fuerzas
		registry->clear();
		// se eliminan las particulas vivas en escena
		particles->kill();

		selectedGen = gen;
		disableAllGenerators();
		if (gen >= Fire1) {
			getParticleGenerator(gen)->init(particles);
		}
		setParticleGenerator(gen, true);
	}

	inline unordered_set<ParticleGenerator*> getActiveGens() {
		unordered_set<ParticleGenerator*> activeGens;
		for (auto generator : particle_generators) {
			if (generator.second) {
				activeGens.insert(generator.first);
			}
		}
		return activeGens;
	}

	inline bool isGenActive(Generators gen) {
		auto activeGens = getActiveGens();
		return activeGens.find(getParticleGenerator(gen)) != activeGens.end();
	}

	// se utiliza para generadores que no son los de los fuegos artificiales
	inline void selectNextGen(Generators ini, Generators fin) {
		if (selectedGen >= ini && selectedGen <= fin) {
			selectedGen = Generators(selectedGen + 1);
			if (selectedGen > fin) {
				selectedGen = ini;
			}
			changeActiveGen(selectedGen);
		}
	}

	inline void toggleForce(ForceParticleGenerator* gen, ForceGens force) {
		if (gen->removeForce(forceGenerators[force])) {
			cout << forceGenerators[force]->getName() << " eliminado" << "\n";
			registry->deleteForceRegistry(forceGenerators[force]);
		}
		else {
			cout << forceGenerators[force]->getName() << " incluido" << "\n";
			gen->addForce(forceGenerators[force], particles);
		}
	}

	void generateForceGens();

	void generateNormalGens();

	void generateFireworkSystem();

public:
	ParticleSystem(Vector3 gravity = Vector3(0, -10, 0));

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
		return particle_generators[gen].first;
	}

	inline void setParticleGenerator(Generators gen, bool enable) {
		particle_generators[gen].second = enable;
	}
};