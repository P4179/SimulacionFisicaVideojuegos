#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>

#include "../skeleton/src/ShootManager.h"
#include "../skeleton/src/ParticleSystem.h"
#include "../skeleton/src/RBStructure/RigidBodySystem.h"
#include "../skeleton/src/Project/Scene.h"

// SE PUEDE ESCRIBIR TEXTO PINTADO
//std::string display_text = "This is a test";

using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;


PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;
ContactReportCallback gContactReportCallback;

ShootManager* shoot = nullptr;
ParticleSystem* particleSystem = nullptr;
RigidBodySystem* RBSystem = nullptr;
Scene* scene = nullptr;

// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
	// método que crear la física del motor de físcia de Physics
	// PxToleranceScale sirve para cambiar la escala de tam y velocidad en toda la simulacion
	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);
	// material que se utiliza para todo (MATERIAL POR DEFECTO)
	// coeficientes de rozamiento
	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	// gravedad definida en la escena
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	// se genera una escena
	// una escena es una simulacion de un conjunto de objetos/actores que puede interaccionar entre ellos
	// puede haber varias escenas, pero cada actor solo puede pertenecer a una de ellas
	gScene = gPhysics->createScene(sceneDesc);

	// CREAR OBJETOS 
	//shoot = new ShootManager();
	//particleSystem = new ParticleSystem();

	// no se elimina porque es un singleton y funciona con puntero inteligente
	//RBSystem = RigidBodySystem::init(gPhysics, gScene);
	//RBSystem->create();

	scene = Scene::init(gPhysics, gScene);
	scene->create();
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds

// t es el deltatime
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);

	// update de los objetos
	// se les pasa el tiempo
	if (shoot != nullptr) {
		shoot->integrate(t);
	}
	if (particleSystem != nullptr) {
		particleSystem->integrate(t);
	}
	if (RBSystem != nullptr) {
		RBSystem->integrate(t);
	}
	if (scene != nullptr) {
		scene->integrate(t);
	}
}

// Function to clean data
// Add custom code to the begining of the function
// liberación de todos los objetos que se han creado
void cleanupPhysics(bool interactive)
{
	// borrar memoria
	if (shoot != nullptr) {
		delete shoot;
	}
	if (particleSystem != nullptr) {
		delete particleSystem;
	}
	if (RBSystem != nullptr) {
		delete RBSystem;
	}
	if (scene != nullptr) {
		delete scene;
	}

	PX_UNUSED(interactive);

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();

	gFoundation->release();
}

// Function called when a key is pressed
// input
// no funcionan todas las teclas
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	if (shoot != nullptr) {
		shoot->keyPressed(toupper(key));
	}
	if (particleSystem != nullptr) {
		particleSystem->keyPressed(toupper(key));
	}
	if (RBSystem != nullptr) {
		RBSystem->keyPressed(toupper(key));
	}
	if (scene != nullptr) {
		scene->keyPressed(toupper(key));
	}
}

// CAMBIO
void keyRelease(unsigned char key) {
	if (scene != nullptr) {
		scene->keyRelease(toupper(key));
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	//PX_UNUSED(actor1);
	//PX_UNUSED(actor2);

	/*if (RBSystem != nullptr) {
		RBSystem->onCollision(actor1, actor2);
	}*/
	if (scene != nullptr) {
		scene->onCollision(actor1, actor2);
	}
}


int main(int, const char* const*)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);	// Check Memory Leaks

#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for (PxU32 i = 0; i < frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}