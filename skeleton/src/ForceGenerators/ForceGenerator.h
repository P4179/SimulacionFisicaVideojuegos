#pragma once
#include "../Particles/Particle.h"

using namespace std;

// CLASE ABSTRACTA
class ForceGenerator {
protected:
	string name;
	double time;
	// si se pone una duración negativa, que el generador
	// de fuerzas es eterno
	double duration;

	ForceGenerator(string name, double duration = -1.0) : name(name), duration(duration), time(0.0) {}

public:
	virtual void updateForce(Particle* particle, double t) = 0;

	// devuelve true si ha pasado todo el tiempo que el generador puede estar vivo
	inline void updateTime(double t) {
		time += t;
	}

	bool isAlive() const {
		return duration < 0 || time < duration;
	}

	inline string getName() const {
		return name;
	}
};