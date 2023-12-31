#pragma once
#include "../../RenderUtils.hpp"
#include <iostream>

class ListParticles;

using namespace std;

// PRACTICA 1-2
enum ParticleType { Default };

/*
Particle es la clase base y se trata de una esfera
- Hijos directos que son particles render con distintas formas
- DynamicRigidBody. Clase abstracta. Tiene hijos con diferentes formas
- Firework
*/
class Particle {
public:
	enum Immersion { Full, Floating, Out };	// para la flotacion

private:
	float radius;

	// vector que contiene todas las fuerzas que se le aplican a la part�cula
	// se combinan todas las fuerzas en una resultante utilizando el principio de superposici�n
	Vector3 force;
	float invMasa;

	// par�metros simulados
	// hay 3 en total: vSimulada, acSimulada y masaSimulada
	float masaSimulada;
	Vector3 acSimulada;

	// vector velocidad, que indica la direcci�n del tiro
	// luego, tiene en cuenta la velocidad simulada
	Vector3 vel;

	// temporizador de la vida
	float elapsedTime;


	void infoParticleType(ParticleType type, float& masaReal, float& vReal);

	// PRACTICA 1
	// type --> velocidad y masas reales
	void calculateSimulatedPhysics(ParticleType type, Vector3 acReal, float vSimulada);

	// se elimina la fuerza resultante
	// las fuerzas son instant�neas, no diferenciales, por lo que
	// hay que borrar la fuerza resultante cada ciclo (al final del update)
	// Por lo tanto, si una de las fuerzas que se aplica a la part�cula
	// es continua, hay que aplicarla cada ciclo
	inline void clearForce() {
		force = Vector3(0);
	}

	inline void commonConstructor(physx::PxShape* shape, Vector4 color, float vSimulada) {
		// geometr�a, posici�n, color (R, G, B, alpha)
		// RGB va desde 0 hasta 1
		// el new ya hace el register
		renderItem = new RenderItem(shape, &pose, color);

		// VELOCIDAD
		// no esta mal usar la velocidad simulada
		// es decir, vel indica la dir y vSimulada la magnitud del vector
		// sin embargo, estaria mejor hace que vel sea el propio vector e incluya todo
		vel = vel.getNormalized();
		this->vel = vel * vSimulada;
	}

protected:
	// coeficiente damping -> se utiliza para solucionar errores de simulaci�n
	// se puede utilizar adem�s para simular rozamiento
	double damping;

	// a render item le pasaremos la direcci�n de este pose, para que se actualize autom�ticamente
	physx::PxTransform pose;
	RenderItem* renderItem;
	Vector4 color;

	// tiempo de vida de la part�cula
	// un tiempo de vida negativo quiere decir que la particula es infinita
	float lifeTime;
	bool alive;

	// PRACTICA 3 -> PARTICULA CUADRADA PARA LA FLOTACION (TIENE UN VOLUMEN Y UNA ALTURA)
	// importa longitud/altura de la particula
	//Vector3 size;
	//float volume;

	Immersion immersion;

	// t est� en segundos
	void updateLifeTime(double t);

	Particle(Vector3 pos, Vector3 vel, float invMasa, double damping, float lifeTime, float vSimulada, physx::PxGeometry* geometry, Vector4 color = Vector4(1, 0, 0, 1));

public:

	Particle(Vector3 pos, Vector3 vel, Vector3 acReal, double damping, float lifeTime, float vSimulada, float radius = 2, Vector4 color = Vector4(1, 0, 0, 1), ParticleType type = Default);

	Particle(Vector3 pos, Vector3 vel, float invMasa, double damping, float lifeTime, float vSimulada, float radius = 2, Vector4 color = Vector4(1, 0, 0, 1));

	virtual ~Particle();

	virtual void integrate(double t);

	inline bool isAlive() const {
		return alive;
	}

	inline void setAlive(bool alive) {
		this->alive = alive;
	}

	inline bool getAlive() const {
		return alive;
	}

	virtual void onDeath(ListParticles* particles) {};

	// a�adir una fuerza a la fuerza resultante
	// se utiliza el principio de superposicion
	virtual inline void addForce(const Vector3& f) {
		force += f;
	}

	virtual inline float getMasa() const {
		// la letra 'e' significa que se trata de notaci�n cient�fica
		// por ejemplo, 1e-10 es 1*10^(-10)
		if (invMasa < 1e-10) {
			throw std::exception("Particle with infinite mass");
		}
		return 1 / invMasa;
	}
	// SOLO PARA LA PARTICULA RENDER
	inline float getInvMasa() const {
		if (invMasa < 1e-10) {
			throw std::exception("Particle with infinite mass");
		}
		return invMasa;
	}
	inline void aumentarMasa() {
		invMasa = invMasa / 10;
		cout << "Inverso masa = " << invMasa << "\n";
	}
	inline void disminuirMasa() {
		float aux = invMasa * 10;
		if (aux < 1) {
			invMasa = aux;
		}
		cout << "Inverso masa = " << invMasa << "\n";
	}

	// SE UTILIZAN PARA LA FLOTACION (IDEALMENTE SOLO SERIA PARA CUBOS)
	virtual inline void disminuirTam() {}
	virtual inline void aumentarTam() {}
	inline void setImmersion(Immersion newImmersion) {
		this->immersion = newImmersion;
	}
	inline Immersion getImmersion() const {
		return immersion;
	}
	// IMPORTANTE REDEFINIR
	virtual inline float getLength() const { return 0; }
	virtual inline float getVolume() const { return 0; }

	// SE UTILIZAN PARA EL VIENTO (REDEFINIR)
	virtual inline float getArea() const {
		return 4 * radius * physx::PxPi;
	}
	virtual inline Vector3 getVel() const {
		return vel;
	}

	// SE UTILIZAN PARA MUCHAS FUERZAS (REDEFINIR)
	virtual inline Vector3 getPos() const {
		return pose.p;
	}
};