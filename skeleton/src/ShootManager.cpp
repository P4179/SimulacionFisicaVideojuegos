#include "ShootManager.h"
#include "checkML.h"

ShootManager::~ShootManager() {
	for (auto& projectile : projectiles) {
		delete projectile;
	}
}

void ShootManager::keyPressed(int __cdecl key) {
	if (key == 'B') {
		if (!projectileShot) {
			shoot();
			projectileShot = true;
		}
	}
}

void ShootManager::integrate(double t) {
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
			if (particle->isAlive()) {
				return false;
			}
			else {
				delete particle;
				return true;
			}
		}), projectiles.end());
}