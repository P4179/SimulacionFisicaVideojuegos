#pragma once
#include "../Particles/Particle.h"
#include <list>
#include <random>

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
	string name;
	// posici�n inicial / origen (luego, se le suma una variabilidad)
	Vector3 _mean_pos;
	// velocidad inicial (luego, se le suma una variabilidad)
	// por ejemplo si queremos que vayan para arriba, ser�a Vector3(0, -1, 0)
	Vector3 _mean_vel;
	// probabilidad de que se generen part�culas o no
	// se determina a partir de _generation_probability > valor devuelto por la distribuci�n _u
	double _generation_probability;
	// n�mero de part�culas que se generan en cada llamada al sistema
	// (podr�a ser un n�mero aleatorio)
	int _num_particles;
	ParticleInfo _info;
	// DISTRIBUCI�N UNIFORME -> se genera un n�mero entre a y b con la misa probabilidad
	// todas las distribuciones dadas un valor x te devuelven un valor y
	// se devuelve un valor que se encuentra dentro del "area" que se crea
	uniform_real_distribution<double> _u;
	// se utiliza para generar n�mero pseudo aleatorios de 32 bits a partir de una semilla
	// se podr�a utilizar std::default_random_engine, que requiere tambi�n de una semilla
	// sin embargo, puede devolver valores inesperados y depende del sistema en el que se ejecute
	// Se va a utilizar para calcular una x aleatoria que pasarle a la distribuci�n
	std::mt19937 _mt;

	// NO SE USA
	// part�cula modelo que se utiliza para generar todas las part�culas del sistema
	// cuando se utiliza, se modifican la posici�n y la velocidad
	Particle* _model;

	ParticleGenerator(string name, Vector3 mean_pos, Vector3 mean_vel, ParticleInfo info, double generation_probability, int num_particles) :
		name(name), _mean_pos(mean_pos), _mean_vel(mean_vel), _info(info), _generation_probability(generation_probability),
		_num_particles(num_particles), _model(nullptr), _u(uniform_real_distribution<double>(0.0, 1.0)) {

		// con random_device se genera un verdadero random, que se va a utilizar como 
		// semilla para calcular luegos numeros pseudo aleatorios
		// no se utiliza todo el rato para calcular n�meros aleatorios porque provoca una llamada al
		// sistema, que es muy costoso
		std::random_device rd;
		// semilla generada por rd
		// (para generar una semilla tambi�n se podr�a utilizar time(NULL), pero es peor)
		_mt = std::mt19937(rd());
	}

public:
	// devuelve una lista de part�culas
	virtual list<Particle*> generateParticles() = 0;

	virtual void update(list<Particle*>& particles) = 0;

	inline void increaseSimulatedV() {
		++_info.vSimulada;
	}

	inline void decreaseSimulatedV() {
		--_info.vSimulada;
		if (_info.vSimulada < 0) {
			_info.vSimulada = 0;
		}
	}

	// NO SE USA
	// cambiar posici�n inicial
	inline void setOrigin(const Vector3& p) {
		_mean_pos = p;
	}
	// cambiar posici�n velocidad inicial
	inline void setMeanVelocity(const Vector3& v) {
		_mean_vel = v;
	}
	inline Vector3 getMeanVelocity() const {
		return _mean_vel;
	}
	// cambiar tiempo de vida de la part�cula modelo
	inline Vector3 setMeanDuration(double newDuration) {
		_model->setLifeTime(newDuration);
	}

	// nuevo modelo de part�cula a partir de la dada
	// la posicion y velocidad iniciales se obtienen a partir de la particula modelo
	inline void setParticle(Particle* p, bool modify_pos_vel = true) {
		if (_model != nullptr) {
			delete _model;
		}
		_model = p->clone();
		if (modify_pos_vel) {
			_mean_pos = p->getPos();
			_mean_vel = p->getVel();
		}
		// posici�n inicial para darle una base
		_model->setPos({ 1000.0f, 1000.0f, 1000.0f });
	}

	// n�mero de part�culas que 
	inline void setNParticles(int n_p) {
		_num_particles = n_p;
	}
};