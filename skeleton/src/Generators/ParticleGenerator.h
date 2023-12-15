#pragma once
#include <list>
#include <random>
#include "../Particles/Particle.h"
#include "../Particles/BoxParticle.h"
#include "../RigidBodies/BoxDynamicRB.h"
#include "../RigidBodies/SphereDynamicRB.h"
#include "../ListParticles.h"
#include "../particle_info.h"
#include "../RBStructure/RigidBodySystem.h"

using namespace std;
using namespace physx;

// CLASE ABSTRACTA
class ParticleGenerator {
protected:
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

	DynamicRigidBody* createRigidBody(Vector3 pos, Vector3 vel) {
		auto system = RigidBodySystem::get();
		switch (_info.geometry) {
		case PxGeometryType::eBOX:
			switch (_info.rigidBody_data.massDef) {
			case Density:
				return new BoxDynamicRB(system->getPhysics(), system->getScene(), pos, vel, Vector3(0, 0, 0), _info.damping,
					_info.rigidBody_data.density_data.density, _info.color, _info.box_data.size, _info.rigidBody_data.material, _info.lifeTime);
				break;
			case InertiaTensor:
				return new BoxDynamicRB(system->getPhysics(), system->getScene(), pos, vel, Vector3(0, 0, 0), _info.damping,
					_info.rigidBody_data.inertiaTensor_data.massDistribution, _info.color, _info.box_data.size, _info.rigidBody_data.material, _info.lifeTime);
				break;
			}
			break;

		case PxGeometryType::eSPHERE:
			switch (_info.rigidBody_data.massDef) {
			case Density:
				return new SphereDynamicRB(system->getPhysics(), system->getScene(), pos, vel, Vector3(0, 0, 0), _info.damping,
					_info.rigidBody_data.density_data.density, _info.color, _info.sphere_data.radius, _info.rigidBody_data.material, _info.lifeTime);
				break;
			case InertiaTensor:
				return new SphereDynamicRB(system->getPhysics(), system->getScene(), pos, vel, Vector3(0, 0, 0), _info.damping,
					_info.rigidBody_data.inertiaTensor_data.massDistribution, _info.color, _info.sphere_data.radius, _info.rigidBody_data.material, _info.lifeTime);
				break;
			}
			break;
		}
	}

	Particle* createParticleRender(Vector3 pos, Vector3 vel) {
		switch (_info.geometry) {
		case  PxGeometryType::eBOX:
			return new BoxParticle(pos, vel, _info.renderParticle_data.invMasa, _info.damping, _info.lifeTime, _info.renderParticle_data.vSimulada, _info.box_data.size, _info.color);
			break;

		case PxGeometryType::eSPHERE:
			return new Particle(pos, vel, _info.renderParticle_data.invMasa, _info.damping, _info.lifeTime, _info.renderParticle_data.vSimulada, _info.sphere_data.radius, _info.color);
			break;
		}
	}

	virtual Particle* createParticle(Vector3 pos, Vector3 vel) {
		if (_info.type == RenderParticle) {
			return createParticleRender(pos, vel);
		}
		else if (_info.type == RigidBody) {
			return createRigidBody(pos, vel);
		}
	}

	inline bool probability() {
		double prob = _u(_mt);
		return _generation_probability > prob;
	}

public:
	virtual ~ParticleGenerator() {}

	// devuelve una lista de partículas
	virtual list<Particle*> generateParticles() = 0;
	
	// solo lo usan los generadores que actualizan las particulas periodicamente
	virtual void update(ListParticles* particles) = 0;

	// si interesara hacer un commit inicial de partículas
	void init(ListParticles* particles) {
		particles->add(generateParticles());
	}

	inline string getName() const {
		return name;
	}

	// SOLO PARA LAS PARTICULAS RENDER
	inline void increaseSimulatedV() {
		++_info.renderParticle_data.vSimulada;
	}
	inline void decreaseSimulatedV() {
		--_info.renderParticle_data.vSimulada;
		if (_info.renderParticle_data.vSimulada < 0) {
			_info.renderParticle_data.vSimulada = 0;
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