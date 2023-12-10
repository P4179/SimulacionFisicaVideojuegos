#pragma once
#include "./Particles/Particle.h"
#include <vector>

using namespace std;

// CANON
// const float V_REAL = 500;
// const float MASA_REAL = 300;

class ShootManager {
private:
	const float TIMER = 2;
	const float V_SIMULADA = 25;

	vector<Particle*> projectiles;
	Camera* camera;
	int numProjectiles;
	bool projectileShot;
	float elapsedTime;

	inline void shoot() {
		// posición de la cámara
		Vector3 pos = camera->getEye();
		auto vel = camera->getDir();
		auto acReal = Vector3(0, -10, 0);
		projectiles.push_back(new Particle(pos, vel, acReal, 0.8, 200, V_SIMULADA));
	}

public:
	ShootManager() : projectiles(), camera(GetCamera()), numProjectiles(0), projectileShot(false), elapsedTime(0) {}

	virtual ~ShootManager();

	void keyPressed(int __cdecl key);

	void integrate(double t);
};