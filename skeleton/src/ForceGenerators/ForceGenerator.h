#pragma once
#include "../Particles/Particle.h"

using namespace std;

template <typename T>
// CLASE ABSTRACTA
class ForceGenerator {
protected:
	string name;
	double elapsedTime;
	// si se pone una duración negativa, que el generador
	// de fuerzas es eterno
	double duration;

	ForceGenerator(string name, double duration = -1.0) : name(name), duration(duration), elapsedTime(0.0) {}

public:
	// virtual para que se llame a la de las clases hijas
	virtual ~ForceGenerator() {}

	virtual void updateForce(T* particle, double t) = 0;

	// devuelve true si ha pasado todo el tiempo que el generador puede estar vivo
	inline void updateTime(double t) {
		elapsedTime += t;
	}

	bool isAlive() const {
		return duration < 0 || elapsedTime < duration;
	}

	inline void resetTime() {
		elapsedTime = 0;
	}

	inline string getName() const {
		return name;
	}
};