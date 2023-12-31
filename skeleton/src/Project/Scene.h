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
// fuerzas
#include "../ForceGenerators/ForceGenerator.h"
#include "../ForceGenerators/BuoyancyForceGenerator.h"
#include "../ForceGenerators/WindForceGenerator.h"
#include "../ForceGenerators/ExplosionForceGenerator.h"
// recursos
#include "../ParticleForceRegistry.h"
#include "../ListParticles.h"
#include "CollisionManager.h"


using namespace physx;
using namespace std;

class Scene : public Singleton<Scene> {
	friend Singleton<Scene>;

private:
	// se consigue un punto cada vez que se llega a una plataforma nueva
	const int MAX_COUNTER = 10;
	const int MAX_PARTICLES = 1000;
	const float WATER_DENSITY = 1000;
	const float END_TIMER = 5;

	float elapsedTime;
	int counter;
	bool dead;

	PxPhysics* gPhysics;
	PxScene* gScene;

	enum TEXT { T_COUNTER, T_END, T_MAX };
	vector<DisplayText*> texts;

	list<std::pair<BoxStaticRB*, bool>> platforms;

	enum GEN { G_MAX };
	list<ForceParticleGenerator*> generators;

	// se van a guardar todas al principio y luego, se van a ir colocando en los lugares correspondientes
	enum FGEN { FG_BUOYANCY, FG_WIND, FG_EXPLOSION, FG_MAX };
	vector<ForceGenerator<Particle>*> forces;
	BuoyancyForceGenerator* buoyancyFg;
	ExplosionForceGenerator<Particle>* explosionFg;

	Character* character;
	Camera* camera;

	ListParticles* listParticles;
	ParticleForceRegistry<Particle>* registry;
	CollisionManager* collisionManager;

	void addText(TEXT enumText, DisplayText* text) {
		texts[enumText] = text;
		RegisterDisplayText(text);
	}

	Scene(PxPhysics* gPhysics, PxScene* gScene) : counter(0), texts(T_MAX, nullptr), platforms(), generators(), forces(FG_MAX, nullptr),
		buoyancyFg(nullptr), character(nullptr), gPhysics(gPhysics), gScene(gScene), camera(GetCamera()), elapsedTime(0), dead(false),
		explosionFg(nullptr) {

		camera->setDir(Vector3(0, 0, -1));

		addText(T_COUNTER, new DisplayText("CONTADOR: " + to_string(MAX_COUNTER - counter), { 1.0f, 0.2f, 0.2f, 1.0f }, { 2,2 }, GLUT_BITMAP_TIMES_ROMAN_24));

		registry = new ParticleForceRegistry<Particle>();
		listParticles = new ListParticles(MAX_PARTICLES, registry);

		createPlatform();
		createForces();

		createPlayer();
	}

	void createPlatform() {
		platforms.push_back({ new BoxStaticRB(gPhysics, gScene, Vector3(0, 0, 0), Vector4(1, 0, 0, 1), Vector3(50, 5, 50), gPhysics->createMaterial(0.5, 0.8, 0)),
			false });
	}

	void createForces() {
		BuoyancyForceGenerator::LiquidVisual visual;
		visual.size = 120;
		visual.color.x = 1;
		forces[FG_BUOYANCY] = new BuoyancyForceGenerator("BuoyancyFg", -100, 30, WATER_DENSITY, visual);

		forces[FG_WIND] = new WindForceGenerator("WindFg", { Vector3(10, 0, 0), Vector3(15, 0, 0), Vector3(20, 0, 0) }, Vector3(0, 10, 0), 10, -1, true, false);

		explosionFg = new ExplosionForceGenerator<Particle>("ExplosionFg", Vector3(0, 10, 0), 10, 30000, 30, -1, true, true);
		forces[FG_EXPLOSION] = explosionFg;
	}

	void createPlayer() {
		character = new Character(gPhysics, gScene, Vector3(0, 70, 0), 0.4, 2, 2);
		registry->addRegistry(forces[FG_BUOYANCY], character);
		//registry->addRegistry(forces[FG_WIND], character);
		registry->addRegistry(forces[FG_EXPLOSION], character);
	}

	void createGenerators() {

	}

	inline void increaseCounter() {
		++counter;
		texts[T_COUNTER]->setText("CONTADOR: " + to_string(MAX_COUNTER - counter));
	}

	inline void setCounter(int newCounter) {
		counter = newCounter;
		texts[T_COUNTER]->setText("CONTADOR: " + to_string(MAX_COUNTER - counter));
	}

public:
	virtual ~Scene() {
		for (auto& text : texts) {
			text->release();
		}
		for (auto& platform : platforms) {
			delete platform.first;
		}
		for (auto& gen : generators) {
			delete gen;
		}
		for (auto& force : forces) {
			delete force;
		}
		delete registry;
		delete listParticles;
		delete character;
		delete collisionManager;
	}

	void processContactPlatform(StaticRigidBody* plat) {
		auto it = std::find_if(platforms.begin(), platforms.end(), [plat](const std::pair<BoxStaticRB*, bool>& platform) {
			return plat == platform.first;
			});

		if (it != platforms.end()) {
			if (!(*it).second) {
				(*it).second = true;
				increaseCounter();
				if (counter > MAX_COUNTER) {
					// fin
				}
			}
		}
	}

	void create() {
		createGenerators();

		vector<BoxStaticRB*> aux(platforms.size());
		int i = 0;
		for (auto platform : platforms) {
			aux[i] = platform.first;
			++i;
		}
		collisionManager = new CollisionManager(character, aux);
	}

	void onCollision(physx::PxActor* actor1, physx::PxActor* actor2) {
		collisionManager->onCollision(actor1, actor2);
	}

	void integrate(double t) {
		collisionManager->integrate(t);

		registry->updateTime(t);

		listParticles->integrate(t);

		listParticles->refresh();

		for (auto gen : generators) {
			gen->update(listParticles);
		}

		character->integrate(t);

		if (character->getImmersion() == Particle::Floating || character->getImmersion() == Particle::Full) {
			dead = true;
		}

		if (dead) {
			elapsedTime += t;
			if (elapsedTime > END_TIMER) {
				for (auto& platform : platforms) {
					platform.second = false;
				}
				dead = false;
				elapsedTime = 0;
				character->resetChar();
				setCounter(0);
			}
		}
	}

	void keyPressed(char key) {
		character->keyPressed(key);

		if (key == 'F') {
			explosionFg->enableExplosion();
		}
	}

	void keyRelease(char key) {
		character->keyRelease(key);
	}
};