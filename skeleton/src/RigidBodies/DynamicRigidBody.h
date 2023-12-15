#pragma once
#include <PxPhysicsAPI.h>
#include "../../RenderUtils.hpp"
#include <iostream>

using namespace physx;
using namespace std;

// Elementos dinamicos en la escena (pueden cambiar su posicion)
class DynamicRigidBody {
protected:
	PxPhysics* gPhysics;
	PxScene* gScene;
	RenderItem* renderItem;
	// hereda de actor
	PxRigidDynamic* dynamicRigid;

	// si es negativa, la vida de la particula es infinita
	float lifeTime;
	float elapsedTime;
	bool alive;

	void commonParam(const Vector3& pos, const Vector3& linearVel, const Vector3& angularVel, float damping, PxGeometry* geometry, PxMaterial* material, const Vector4& color) {
		// fisicas y posicion del objeto estatico
		dynamicRigid = gPhysics->createRigidDynamic(PxTransform(pos));
		// velocidad con la que un objeto se desplaza un objeto a
		// lo largo de una trayectoria recta/punto A a punto B (m/s)
		dynamicRigid->setLinearVelocity(linearVel);
		// velocidad a la que rota un objeto alrededor de un punto/eje (rad/s)
		// por ejemplo, velocidad a la que rota una rueda de un coche alrededor de su eje
		dynamicRigid->setAngularVelocity(angularVel);
		// NO SE SI HACE FALTA
		dynamicRigid->setLinearDamping(damping);
		// se crea la forma con su material a partir de una geometria
		PxShape* shape = CreateShape(*geometry, material);
		// forma del objeto, que afecta a las fisicas
		dynamicRigid->attachShape(*shape);
		// se pone el elem en la escena, pero luego hay que pintarlo aparte
		gScene->addActor(*dynamicRigid);
		// se pinta el objeto
		renderItem = new RenderItem(shape, dynamicRigid, color);
	}

	DynamicRigidBody(PxPhysics* gPhysics, PxScene* gScene, const Vector3& pos,
		const Vector3& linearVel, const Vector3& angularVel, float damping, float density, Vector4 color,
		PxGeometry* geometry, PxMaterial* material = nullptr, float lifeTime = -1) :
		gPhysics(gPhysics), gScene(gScene), lifeTime(lifeTime), elapsedTime(0), alive(true) {

		commonParam(pos, linearVel, angularVel, damping, geometry, material, color);
		// 1ª FORMA DE DEFINIR LA MASA
		// se hace a traves de una densidad (kg/m^3)
		// la masa se distribuye de forma uniforme
		// por debajo funcion como una un tensor de inercia
		PxRigidBodyExt::updateMassAndInertia(*dynamicRigid, density);
	}

	DynamicRigidBody(PxPhysics* gPhysics, PxScene* gScene, const Vector3& pos,
		const Vector3& linearVel, const Vector3& angularVel, float damping, Vector3 massDistribution, Vector4 color,
		PxGeometry* geometry, PxMaterial* material = nullptr, float lifeTime = -1) :
		gPhysics(gPhysics), gScene(gScene), lifeTime(lifeTime), elapsedTime(0), alive(true) {

		commonParam(pos, linearVel, angularVel, damping, geometry, material, color);
		// 2ª FORMA DE DEFINIR LA MASA
		// se hace a traves de un tensor de inercia
		// un tensor de inercia describe como esta distribuida la masa de un objeto
		// a partir de sus ejes de rotacion
		// se trata de una matrix3x3, pero para que sea mas facil usarlo,
		// PhysX utiliza una matriz diagonal
		/*
		| mx  0  0 |
		| 0  my  0 | ==> (mx, my, mz)
		| 0   0 mz |
		*/
		dynamicRigid->setMassSpaceInertiaTensor(massDistribution);
	}

public:
	virtual ~DynamicRigidBody() {
		renderItem->release();
		dynamicRigid->release();
	}

	inline Vector3 getPos() const {
		Vector3 pos = dynamicRigid->getGlobalPose().p;
		//cout << "X: " << pos.x << " Y: " << pos.y << " Z: " << pos.z << "\n";
		return pos;
	}

	inline void addForce(const Vector3& force) {
		dynamicRigid->addForce(force);
	}

	// se trata de la fuerza de torsion
	// si se aplica, hace que un objeto gire en torno a su(s) eje(s)
	inline void addTorque(const Vector3& torque) {
		dynamicRigid->addTorque(torque);
	}

	inline PxRigidActor* getActor() const {
		return dynamicRigid;
	}

	inline void updateLifeTime(double t) {
		if (lifeTime >= 0) {
			elapsedTime += t;
			if (elapsedTime > lifeTime) {
				elapsedTime = 0;
				alive = false;
			}
		}
	}

	inline bool isAlive() const {
		return alive;
	}
};