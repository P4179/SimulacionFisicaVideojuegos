#include "RigidBodyGenerator.h"
#include "../RBStructure/RigidBodySystem.h"

DynamicRigidBody* RigidBodyGenerator::createRigidBody(Vector3 pos, Vector3 vel) {
	switch (RBfeat.geometry) {
	case PxGeometryType::eBOX:
		switch (RBfeat.rigidBody_data.massDef) {
		case Density:
			return new BoxDynamicRB(system->getPhysics(), system->getScene(), pos, vel, Vector3(0, 0, 0), RBfeat.damping,
				RBfeat.rigidBody_data.density_data.density, RBfeat.color, RBfeat.box_data.size, RBfeat.rigidBody_data.material, RBfeat.lifeTime);
			break;
		case InertiaTensor:
			return new BoxDynamicRB(system->getPhysics(), system->getScene(), pos, vel, Vector3(0, 0, 0), RBfeat.damping,
				RBfeat.rigidBody_data.inertiaTensor_data.massDistribution, RBfeat.color, RBfeat.box_data.size, RBfeat.rigidBody_data.material, RBfeat.lifeTime);
			break;
		}
		break;

	case PxGeometryType::eSPHERE:
		switch (RBfeat.rigidBody_data.massDef) {
		case Density:
			return new SphereDynamicRB(system->getPhysics(), system->getScene(), pos, vel, Vector3(0, 0, 0), RBfeat.damping,
				RBfeat.rigidBody_data.density_data.density, RBfeat.color, RBfeat.sphere_data.radius, RBfeat.rigidBody_data.material, RBfeat.lifeTime);
			break;
		case InertiaTensor:
			return new SphereDynamicRB(system->getPhysics(), system->getScene(), pos, vel, Vector3(0, 0, 0), RBfeat.damping,
				RBfeat.rigidBody_data.inertiaTensor_data.massDistribution, RBfeat.color, RBfeat.sphere_data.radius, RBfeat.rigidBody_data.material, RBfeat.lifeTime);
			break;
		}
		break;
	}
}

RigidBodyGenerator::RigidBodyGenerator(RBGens gen, Vector3 meanPos, Vector3 meanVel, double genProbability, int numParticles, ParticleInfo RBfeat) :
	gen(gen), meanPos(meanPos), meanVel(meanVel), genProbability(genProbability), numParticles(numParticles), RBfeat(RBfeat),
	u(uniform_real_distribution<double>(0.0, 1.0)) {

	system = RigidBodySystem::get();
	std::random_device rd;
	mt = std::mt19937(rd());
}