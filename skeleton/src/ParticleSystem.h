#pragma once
#include <unordered_map>
#include <functional>
// generadores de particulas
#include "./Generators/UniformParticleGenerator.h"
#include "./Generators/GaussianParticleGenerator.h"
#include "./Generators/FireworkGenerator.h"
#include "./Generators/ParticleGenerator.h"
#include "./Generators/CircleGenerator.h"
#include "./Generators/ForceParticleGenerator.h"
#include "./Generators/LauncherParticleGenerator.h"
// otros
#include "./ListParticles.h"
#include "ParticleForceRegistry.h"
// generadores de fuerza
#include "./ForceGenerators/GravityForceGenerator.h"
#include "./ForceGenerators/WindForceGenerator.h"
#include "./ForceGenerators/WhirlwindForceGenerator.h"
#include "./ForceGenerators/ExplosionForceGenerator.h"
	// Practica 3
#include "./ForceGenerators/AnchoredSpringForceGenerator.h"
#include "./ForceGenerators/BuoyancyForceGenerator.h"
#include "./ForceGenerators/ElasticRubberForceGenerator.h"
#include "./ForceGenerators/SpringForceGenerator.h"

using namespace std;

// el último valor sirve para saber cuantos generadores hay en total
enum Generators {
	// generadores que se ejecutan cada vez
	Fuente, Manguera, LLuvia, MangueraGaussiana, Niebla,
	FuerzaDefecto, Viento1, Viento2, Torbellino, Explosion,
	// lanzador de particulas, es decir, asigna cada particula con su fuerza
	Launch,
	// generadores que se lanzan al principio o solo sirven para propagar
	Fire1, Fire2, Fire3, Fire4, Circle, MAX, NONE
};

enum ForceGens {
	GravityGen, WindGen, WhirlWindGen, ExplosionGen,
	// PRACTICA 3 --> cada vez que se cambia de generador, si estan creadas, se eliminan
	// Se hace asi porque tienen objetos propios que se seguirian mostrando en escena
	AnchorGen, SpringGen1, SpringGen2, ElasticRubber1, ElasticRubber2, BuoyanceGen,
	MAX_FORCES
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
	{Launch, "Launch"},
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
	ExplosionForceGenerator* explosionGen;
	Generators selectedGen;

	vector<ForceGenerator*> forceGenerators;
	ParticleForceRegistry* registry;

	// guarda el generador de fuerzas por defecto de la practica 2, al que se le ponen o quitan fuerzas
	ForceParticleGenerator* fuerzaDefGenParticula;
	// guarda el generador al que se le van a poner o quitar fuerzas ahora
	ForceParticleGenerator* cambiarFuerzasGen;
	// guarda el lanzador de particulas en una variable para poder usarlo cuando se quiera
	LauncherParticleGen* launcherParticle;
	SpringForceGenerator* springFg;

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

	inline void launch(std::function<void(vector<std::pair<ForceGenerator*, Particle*>>& forceParticles, vector<ForceGenerator*>& forceGens)> function) {
		// importa el orden
		changeActiveGen(Launch);
		vector<std::pair<ForceGenerator*, Particle*>> forcesParticles = {};
		vector<ForceGenerator*> forceGens = {};
		// rellenar los dos vectores
		function(forcesParticles, forceGens);
		launcherParticle->launch(particles, forcesParticles, forceGens);
		cambiarFuerzasGen = launcherParticle;
	}

	inline void disableAllGenerators() {
		for (auto& generator : particle_generators) {
			generator.second = false;
		}
	}

	inline void removeSpringSystem() {
		launcherParticle->clear();
		for (ForceGens fGen = AnchorGen; fGen <= BuoyanceGen; fGen = ForceGens(fGen + 1)) {
			if (forceGenerators[fGen] != nullptr) {
				delete forceGenerators[fGen];
				forceGenerators[fGen] = nullptr;
			}
		}
		springFg = nullptr;
	}

	inline void changeActiveGen(Generators gen) {
		// ELIMINADO
		// se deregistran particulas y fuerzas
		registry->clear();
		// se eliminan las particulas vivas en escena
		particles->kill();
		removeSpringSystem();
		disableAllGenerators();

		selectedGen = gen;
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

	// puede haber varios generadores activos a la vez
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

	void generateAnchorSystem(vector<std::pair<ForceGenerator*, Particle*>>& forceParticles, vector<ForceGenerator*>& forceGens);

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