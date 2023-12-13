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
	FuenteGen, MangueraGen, LLuviaGen, MangueraGaussianaGen, NieblaGen,
	AplicarFuerzaGen, Viento1Gen, Viento2Gen, TorbellinoGen, ExplosionGen,
	// lanzador de particulas, es decir, asigna cada particula con su fuerza
	LaunchGen,
	// generadores que se lanzan al principio o solo sirven para propagar
	Fire1Gen, Fire2Gen, Fire3Gen, Fire4Gen, CircleGen, MAX, NONE
};

enum ForceGens {
	GravityFg, WindFg, WhirlWindFg, ExplosionFg,
	// PRACTICA 3 --> cada vez que se cambia de generador, si estan creadas, se eliminan
	// Se hace asi porque tienen objetos propios que se seguirian mostrando en escena
	AnchorFg, Spring1Fg, Spring2Fg, ElasticRubber1Fg, ElasticRubber2Fg,
	Slinky1Fg, Slinky2Fg, Slinky3Fg, Slinky4Fg, Slinky5Fg,
	Slinky1BisFg, Slinky2BisFg, Slinky3BisFg, Slinky4BisFg, Slinky5BisFg,
	BuoyanceFg,
	MAX_FORCES
};

const unordered_map<Generators, string> generatorsNames{
	{FuenteGen, "FuenteGen"},
	{MangueraGen, "MangueraGen"},
	{LLuviaGen, "LLuviaGenGen"},
	{MangueraGaussianaGen, "MangueraGaussianaGen"},
	{NieblaGen, "NieblaGen"},
	{AplicarFuerzaGen,"AplicarFuerzaGen"},
	{Viento1Gen, "Viento1Gen"},
	{Viento2Gen, "Viento2Gen"},
	{TorbellinoGen, "TorbellinoGen"},
	{ExplosionGen, "ExplosionGen"},
	{LaunchGen, "LaunchGen"},
	{Fire1Gen, "Fire1Gen"},
	{Fire2Gen,"Fire2Gen"},
	{Fire3Gen,"Fire3Gen"},
	{Fire4Gen,"Fire4Gen"},
	{CircleGen,"CircleGen"}
};

class ParticleSystem {
private:
	const float DAMPING = 0.8;
	const float WATER_DENSITY = 1000;

	ListParticles* particles;
	// se utiliza para actualizar los generadores y para obtener cada generador
	vector<std::pair<ParticleGenerator*, bool>> particle_generators;
	unordered_map<string, ParticleGenerator*> particleGenFindByName;
	Vector3 gravity;
	Generators selectedGen;

	vector<ForceGenerator<Particle>*> forceGenerators;
	ParticleForceRegistry<Particle>* registry;

	GaussianParticleGenerator* mangueraGaussianGen;
	ExplosionForceGenerator<Particle>* explosionFg;
	// guarda el generador de fuerzas por defecto de la practica 2, al que se le ponen o quitan fuerzas
	ForceParticleGenerator* aplicarFuerzaGen;
	// guarda el generador al que se le van a poner o quitar fuerzas ahora
	ForceParticleGenerator* cambiarFuerzasGen;
	// guarda el lanzador de particulas en una variable para poder usarlo cuando se quiera
	LauncherParticleGen* launcherParticleGen;
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

	inline void launch(std::function<void(vector<std::pair<ForceGenerator<Particle>*, Particle*>>& forceParticles, unordered_set<ForceGenerator<Particle>*>& forceGens)> function) {
		// importa el orden
		changeActiveGen(LaunchGen);
		vector<std::pair<ForceGenerator<Particle>*, Particle*>> forcesParticles = {};
		unordered_set<ForceGenerator<Particle>*> forceGens = {};
		// rellenar los dos vectores
		function(forcesParticles, forceGens);
		// el generador vincula particulas y fuerzas, es decir, hace el commit inicial
		launcherParticleGen->launch(particles, forcesParticles, forceGens);
		cambiarFuerzasGen = launcherParticleGen;
	}

	inline void disableAllGenerators() {
		for (auto& generator : particle_generators) {
			generator.second = false;
		}
	}

	inline void removeSpringSystem() {
		launcherParticleGen->clear();
		for (ForceGens fGen = AnchorFg; fGen <= BuoyanceFg; fGen = ForceGens(fGen + 1)) {
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
		if (gen >= Fire1Gen) {
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

	void generateAnchorSystem(vector<std::pair<ForceGenerator<Particle>*, Particle*>>& forceParticles, unordered_set<ForceGenerator<Particle>*>& forceGens);

	void generateSpringSystem(vector<std::pair<ForceGenerator<Particle>*, Particle*>>& forceParticles, unordered_set<ForceGenerator<Particle>*>& forceGens);

	void generateElasticRubberSystem(vector<std::pair<ForceGenerator<Particle>*, Particle*>>& forceParticles, unordered_set<ForceGenerator<Particle>*>& forceGens);

	void generateSlinkySystem(vector<std::pair<ForceGenerator<Particle>*, Particle*>>& forceParticles, unordered_set<ForceGenerator<Particle>*>& forceGens);

	void generateBuoyancySystem(vector<std::pair<ForceGenerator<Particle>*, Particle*>>& forceParticles, unordered_set<ForceGenerator<Particle>*>& forceGens);

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