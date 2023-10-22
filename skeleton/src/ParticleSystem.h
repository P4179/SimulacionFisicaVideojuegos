#pragma once
#include <list>
#include <unordered_map>
#include "./Generators/UniformParticleGenerator.h"
#include "./Generators/GaussianParticleGenerator.h"
#include "./Generators/FireworkGenerator.h"
#include "./Generators/ParticleGenerator.h"
#include "./ListParticles.h"

using namespace std;

// el último valor sirve para saber cuantos generadores hay en total
enum Generators {
	Fuente, Manguera, LLuvia, MangueraGaussiana, Niebla,
	Fire1, Fire2, Fire3, Fire4, Max
};

const vector<string> generatorsNames {
	"Fuente", "Manguera", "LLuvia", "MangueraGaussiana", "Niebla",
		"Fire1", "Fire2", "Fire3", "Fire4"
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
	ParticleSystem(Vector3 gravity = Vector3(0, -10, 0)) : gravity(gravity), selectedGen(Fire1),
		particle_generators(vector<ParticleGenerator*>(Max)) {

		particles = new ListParticles(1000);

		ParticleInfo info;
		info.ac = gravity;
		info.damping = DAMPING;

		info.color = Vector4(0.337, 0.192, 0.8, 1);
		info.lifeTime = 20;
		info.radius = 1.4;
		info.vSimulada = 45;
		// pos original, velocidad original, info particula, probabilidad, numero particulas, variacion velocidad, variacion posicion
		addParticleGenerator<UniformParticleGenerator>(Fuente, Vector3(0, 0, 0), Vector3(0, 1, 0), info, 0.3, 1, Vector3(0.2, 0, 0.2), Vector3(2, 0, 2));

		info.color = Vector4(0.18, 0.596, 0.839, 1);
		info.lifeTime = 20;
		info.radius = 1;
		info.vSimulada = 40.0;
		addParticleGenerator<UniformParticleGenerator>(Manguera, Vector3(-40, 10, 0), Vector3(0.6, 1, 0), info, 0.3, 1, Vector3(0, 0.3, 0.2), Vector3(1, 0, 1));

		info.color = Vector4(0.361, 0.529, 0.729, 1);
		info.lifeTime = 20;
		info.radius = 0.6;
		info.vSimulada = 35.0;
		addParticleGenerator<UniformParticleGenerator>(LLuvia, Vector3(0, 65, 0), Vector3(0, -1, 0), info, 0.5, 3, Vector3(0, 0, 0), Vector3(20, 2, 20));

		info.color = Vector4(0.165, 0.702, 0.91, 1);
		info.lifeTime = 20;
		info.radius = 0.8;
		info.vSimulada = 45.0;
		// la vel inicial y las direcciones de velocidad solo sirven para indicar cual incrementa más y cual incrementa menos
		// pues luego, se normalizara el vector y se multiplicar por la vSimulada
		mangueraGaussiana = addParticleGenerator<GaussianParticleGenerator>(MangueraGaussiana, Vector3(-60, 10, 0), Vector3(0.7, 0.4, 0), info, 0.5, 3, Vector3(0.03, 0.02, 0.05), Vector3(0, 0, 0.5));

		info.color = Vector4(0.518, 0.635, 0.678, 0.7);
		info.lifeTime = 20;
		info.radius = 0.7;
		info.vSimulada = 8;
		// la vel inicial y las direcciones de velocidad solo sirven para indicar cual incrementa más y cual incrementa menos
		// pues luego, se normalizara el vector y se multiplicar por la vSimulada
		addParticleGenerator<GaussianParticleGenerator>(Niebla, Vector3(0, 30, 0), Vector3(0, 1, 0), info, 0.7, 4, Vector3(2.5, 0.02, 2.5), Vector3(15, 12, 15));

		generateFireworkSystem();
	}

	void generateFireworkSystem() {
		ParticleInfo info;
		info.ac = gravity;
		info.damping = DAMPING;

		info.color = Vector4(0.89, 0.09, 0.29, 1);
		info.lifeTime = 1.8;
		info.radius = 1.3;
		info.vSimulada = 33.0;
		// pos original, velocidad original, info particula, numero particulas, variacion velocidad, variacion posicion
		auto fireGen = addParticleGenerator<FireworkGenerator>(Fire1, Vector3(0, 0, 0), Vector3(0, 1, 0), info, 2, Vector3(0.1, 0.2, 0.1), Vector3(1, 1, 1));

		info.color = Vector4(0.196, 0.871, 0.153, 1);
		info.lifeTime = 1.7;
		info.radius = 0.9;
		info.vSimulada = 20.0;
		// pos original, velocidad original, info particula, numero particulas, variacion velocidad, variacion posicion
		fireGen = addParticleGenerator<FireworkGenerator>(Fire2, Vector3(0, 20, 0), Vector3(0, 1, 0), info, 4, Vector3(0.8, 0.1, 0.8), Vector3(1, 1, 1));
		FireworkGenerator::addFireworkGen(fireGen);

		info.color = Vector4(0.612, 0.216, 0.91, 1);
		info.lifeTime = 2.2;
		info.radius = 1;
		info.vSimulada = 25.0;
		// pos original, velocidad original, info particula, numero particulas, variacion velocidad, variacion posicion
		fireGen = addParticleGenerator<FireworkGenerator>(Fire3, Vector3(0, 20, 0), Vector3(0.8, 0.8, 0), info, 2, Vector3(0.3, 0, 0.5), Vector3(1, 1, 1));
		FireworkGenerator::addFireworkGen(fireGen);

		info.color = Vector4(0.961, 0.118, 0.808, 1);
		info.lifeTime = 2.0;
		info.radius = 0.8;
		info.vSimulada = 30.0;
		// pos original, velocidad original, info particula, numero particulas, variacion velocidad, variacion posicion
		fireGen = addParticleGenerator<FireworkGenerator>(Fire4, Vector3(0, 20, 0), Vector3(-0.7, 0.6, 0), info, 3, Vector3(0.4, 0.2, 0.1), Vector3(1, 1, 1));
		FireworkGenerator::addFireworkGen(fireGen);

		// idealmente se inicia con FireworkGen1, aunque se podría iniciar con cualquiera
		if (selectedGen >= Fire1) {
			particles->add(getParticleGenerator(selectedGen)->generateParticles());
		}
	}

	virtual ~ParticleSystem() {
		delete particles;
		for (auto& gen : particle_generators) {
			delete gen;
		}
		particle_generators.clear();
	}

	void integrate(double t) {
		particles->refresh();

		// update de cada partícula
		particles->integrate(t);

		// generar más partículas
		// un generador de partículas de devuelve una lista de partículas
		// y se mete en la lista de partículas del sistema
		try {
			getParticleGenerator(selectedGen)->update(particles);
		}
		catch (exception e) {
			cout << e.what() << "\n";
			exit(EXIT_FAILURE);
		}
	}

	void keyPressed(int __cdecl key) {
		bool changeGen = false;
		Generators newGen;

		switch (key) {
		case 'Z':
			newGen = Fuente;
			changeGen = true;
			break;
		case 'X':
			newGen = LLuvia;
			changeGen = true;
			break;
		case 'C':
			newGen = Manguera;
			changeGen = true;
			break;
		case 'V':
			newGen = MangueraGaussiana;
			changeGen = true;
			break;
		case 'B':
			newGen = Niebla;
			changeGen = true;
			break;
		case 'H':
			if (selectedGen == MangueraGaussiana) {
				getParticleGenerator(MangueraGaussiana)->decreaseSimulatedV();
			}
			break;
		case 'J':
			if (selectedGen == MangueraGaussiana) {
				getParticleGenerator(MangueraGaussiana)->increaseSimulatedV();
			}
			break;
		case 'K':
			if (selectedGen == MangueraGaussiana) {
				auto stdDevVel = mangueraGaussiana->getStdDevVel();
				stdDevVel.z -= 0.1;
				if (stdDevVel.z < 0) {
					stdDevVel.z = 0;
				}
				mangueraGaussiana->setStdDevVel(stdDevVel);
			}
			break;
		case 'L':
			if (selectedGen == MangueraGaussiana) {
				auto stdDevVel = mangueraGaussiana->getStdDevVel();
				stdDevVel.z += 0.1;
				mangueraGaussiana->setStdDevVel(stdDevVel);
			}
			break;
		}

		if (changeGen && newGen != selectedGen) {
			particles->kill();
			selectedGen = newGen;
		}
	}

	inline ParticleGenerator* ParticleSystem::getParticleGenerator(string name) const {
		auto it = particleGenFindByName.find(name);
		if (it == particleGenFindByName.end()) {
			throw exception("It does not exist a particle generator with that name");
		}
		else {
			return (*it).second;
		}
	}

	inline ParticleGenerator* ParticleSystem::getParticleGenerator(Generators gen) const {
		return particle_generators[gen];
	}
};