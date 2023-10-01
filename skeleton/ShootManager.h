#pragma once
#include "Particle.h"
#include <vector>

using namespace std;

// CANON
const float V_REAL = 500;
const float MASA_REAL = 300;

const float V_SIMULADA = 25;
const float TIMER = 2;

class ShootManager {
private:
	vector<Particle*> projectiles;
	Camera* camera;
	int numProjectiles;
	bool projectileShot;
	float elapsedTime;

	void shoot() {
		// posición de la cámara
		Vector3 pos = camera->getEye();
		auto vel = camera->getDir() * V_SIMULADA;
		projectiles.push_back(new Particle(pos, vel, 0.8, MASA_REAL, V_REAL, V_SIMULADA));
	}

public:
	ShootManager() : projectiles(), camera(GetCamera()), numProjectiles(0), projectileShot(false), elapsedTime(0) {}

	~ShootManager() {
		for (auto& projectile : projectiles) {
			delete projectile;
		}
	}

	void keyPressed(int __cdecl key) {
		if (key == 'B') {
			if (!projectileShot) {
				shoot();
				projectileShot = true;
			}
		}
	}

	void integrate(double t) {
		for (auto projectile : projectiles) {
			projectile->integrate(t);
		}

		if (projectileShot) {
			elapsedTime += t;
			if (elapsedTime > TIMER) {
				elapsedTime = 0;
				projectileShot = false;
			}
		}

		// erase elimina los elementos de un rango y reduce el tamaño del vector
		// remove_if no es capaz de cambiar el tamaño, por lo que reemplaza el elemento que tiene que eliminar
		// por el siguiente y devuelve un iterador al final del "nuevo" vector
		projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(),
			[](Particle* particle) {
				if (particle->getAlive()) {
					return false;
				}
				else {
					delete particle;
					return true;
				}
			}), projectiles.end());
	}
};