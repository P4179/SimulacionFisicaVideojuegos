#pragma once
#include <list>
#include <random>
#include "../Particles/Particle.h"
#include "../ListParticles.h"

using namespace std;

struct ParticleInfo {
	Vector3 ac;
	float damping;
	float lifeTime;
	float vSimulada;
	float radius;
	Vector4 color;
};

// CLASE ABSTRACTA
class ParticleGenerator {
protected:
	const int NUM_MAX_PARTICLES = 100;

	string name;
	// posición inicial / origen (luego, se le suma una variabilidad)
	Vector3 _mean_pos;
	// velocidad inicial (luego, se le suma una variabilidad)
	// por ejemplo si queremos que vayan para arriba, sería Vector3(0, -1, 0)
	Vector3 _mean_vel;
	// probabilidad de que se generen partículas o no
	// se determina a partir de _generation_probability > valor devuelto por la distribución _u
	double _generation_probability;
	// número de partículas que se generan en cada llamada al sistema
	// (podría ser un número aleatorio)
	int _num_particles;
	ParticleInfo _info;
	// DISTRIBUCIÓN UNIFORME -> se genera un número entre a y b con la misa probabilidad
	// todas las distribuciones dadas un valor x te devuelven un valor y
	// se devuelve un valor que se encuentra dentro del "area" que se crea
	uniform_real_distribution<double> _u;
	// se utiliza para generar número pseudo aleatorios de 32 bits a partir de una semilla
	// se podría utilizar std::default_random_engine, que requiere también de una semilla
	// sin embargo, puede devolver valores inesperados y depende del sistema en el que se ejecute
	// Se va a utilizar para calcular una x aleatoria que pasarle a la distribución
	std::mt19937 _mt;

	ParticleGenerator::ParticleGenerator(string name, Vector3 mean_pos, Vector3 mean_vel, ParticleInfo info, double generation_probability, int num_particles) :
		name(name), _mean_pos(mean_pos), _mean_vel(mean_vel), _info(info), _generation_probability(generation_probability),
		_num_particles(num_particles), _u(uniform_real_distribution<double>(0.0, 1.0)) {

		// con random_device se genera un verdadero random, que se va a utilizar como 
		// semilla para calcular luegos numeros pseudo aleatorios
		// no se utiliza todo el rato para calcular números aleatorios porque provoca una llamada al
		// sistema, que es muy costoso
		std::random_device rd;
		// semilla generada por rd
		// (para generar una semilla también se podría utilizar time(NULL), pero es peor)
		_mt = std::mt19937(rd());
	}

public:
	// devuelve una lista de partículas
	virtual list<Particle*> generateParticles() = 0;

	// solo lo usan los generadores que actualizan las particulas periodicamente
	virtual void update(ListParticles* particles) = 0;

	// si interesara hacer un commit inicial de partículas
	void init(ListParticles* particles) {
		particles->add(generateParticles());
	}

	inline void increaseSimulatedV() {
		++_info.vSimulada;
	}

	inline void decreaseSimulatedV() {
		--_info.vSimulada;
		if (_info.vSimulada < 0) {
			_info.vSimulada = 0;
		}
	}

	// cambiar posición inicial
	inline void setOrigin(const Vector3& p) {
		_mean_pos = p;
	}

	inline void setProbability(float newProbability) {
		this->_generation_probability = newProbability;
	}
};