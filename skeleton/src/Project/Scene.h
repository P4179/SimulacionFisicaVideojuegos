#pragma once
#include <PxPhysicsAPI.h>
#include "../../RenderUtils.hpp"
#include "../Singleton.h"
#include <vector>
#include <list>
// actores
#include "Character.h"
#include "../RigidBodies/StaticRigidBody.h"
// generadores
#include "../Generators/ForceParticleGenerator.h"
#include "../Generators/FireworkGenerator.h"
#include "../Generators/CircleGenerator.h"
#include "../Generators/UniformParticleGenerator.h"
// fuerzas
#include "../ForceGenerators/ForceGenerator.h"
#include "../ForceGenerators/BuoyancyForceGenerator.h"
#include "../ForceGenerators/WindForceGenerator.h"
#include "../ForceGenerators/ExplosionForceGenerator.h"
#include "../ForceGenerators/GravityForceGenerator.h"
// recursos
#include "../ParticleForceRegistry.h"
#include "../ListParticles.h"
#include "CollisionManager.h"


using namespace physx;
using namespace std;

class Scene : public Singleton<Scene> {
	friend Singleton<Scene>;
public:
	// ACTIVAR/DESACTIVAR AREAS DE FUERZAS Y RENDER CAPSULA PERSONAJE
	const bool DEBUG = true;

private:
	// se consigue un punto cada vez que se llega a una plataforma nueva
	const int MAX_COUNTER = 10;
	const int MAX_PARTICLES = 2000;
	const float WATER_DENSITY = 1000;
	const float RESET_TIMER = 5;
	const Vector3 GRAVITY = Vector3(0, -10, 0);
	const float DAMPING = 0.8;
	const float FIREWORK_OFFSET = 80;
	const float FIREWORK_HEIGHT = 25;
	const float HALF_AREA = -373;
	const Vector3 PLATFORM_SIZE = Vector3(60, 5, 60);
	const float EXPLOSION_TIMER = 6;

	float elapsedTimeEnd;
	int counter;
	bool gameover;
	bool win;

	physx::PxPhysics* gPhysics;
	physx::PxScene* gScene;

	enum TEXT { T_COUNTER, T_WIN, T_GAMEOVER, T_MAX };
	vector<DisplayText*> texts;

	list<std::pair<BoxStaticRB*, bool>> platforms;

	list<ForceParticleGenerator*> generators;
	CircleGenerator* fireworkLauncher;

	// se van a guardar todas al principio y luego, se van a ir colocando en los lugares correspondientes
	enum FGEN {
		FG_BUOYANCY, FG_WIND_LEFT, FG_WIND_UP, FG_EXPLOSION, FG_GRAVITY, FG_MAX
	};
	vector<ForceGenerator<Particle>*> forces;
	BuoyancyForceGenerator* buoyancyFg;
	ExplosionForceGenerator<Particle>* explosionFg;
	float elapsedTimeExplosion;

	Character* character;
	Camera* camera;

	ListParticles* listParticles;
	ParticleForceRegistry<Particle>* registry;
	CollisionManager* collisionManager;

	inline void addFireworkGen(bool spread, FireworkGenerator* fireworkGen) {
		generators.push_back(fireworkGen);
		if (spread) {
			FireworkGenerator::addFireworkGen(fireworkGen);
		}
	}

	void createTexts();

	void createPlatform();

	void createForces();

	void createPlayer();

	void createGenerators();

	void createFireworks();

	inline void increaseCounter() {
		if (counter < MAX_COUNTER) {
			++counter;
			texts[T_COUNTER]->setText("CONTADOR: " + to_string(MAX_COUNTER - counter));
		}
	}

	inline void setCounter(int newCounter) {
		counter = newCounter;
		texts[T_COUNTER]->setText("CONTADOR: " + to_string(MAX_COUNTER - counter));
	}

	void reset() {
		character->resetChar();
		for (auto& platform : platforms) {
			platform.second = false;
		}
		win = false;
		gameover = false;
		elapsedTimeEnd = 0;
		setCounter(0);
		listParticles->kill();
		explosionFg->disableExplosion();
		elapsedTimeExplosion = 0;
	}

	inline void hasWinned() {
		RegisterDisplayText(texts[T_WIN]);
		character->setInput(false);
		fireworkLauncher->setOrigin(camera->getEye() + camera->getDir() * FIREWORK_OFFSET + Vector3(0, FIREWORK_HEIGHT, 0));
		Vector3 aux = camera->getLeftDir();	aux.x = -aux.x;
		fireworkLauncher->setDirs(camera->getUpDir(), aux);
		fireworkLauncher->init(listParticles);
		win = true;
	}

	inline void hasLost() {
		gameover = true;
		RegisterDisplayText(texts[T_GAMEOVER]);
	}

	Scene(physx::PxPhysics* gPhysics, physx::PxScene* gScene);

public:
	virtual ~Scene();

	void processContactPlatform(StaticRigidBody* plat);

	void create();

	void onCollision(physx::PxActor* actor1, physx::PxActor* actor2) {
		collisionManager->onCollision(actor1, actor2);
	}

	void integrate(double t);

	void keyPressed(char key) {
		character->keyPressed(key);
	}

	void keyRelease(char key) {
		character->keyRelease(key);
	}
};