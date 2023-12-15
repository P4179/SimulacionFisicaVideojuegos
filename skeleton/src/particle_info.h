#pragma once
#include "../RenderUtils.hpp"

using namespace physx;

enum MassDef { InvMasa, Density, InertiaTensor };

struct ParticleInfo {
	// no se utiliza en la practica 3
	//Vector3 ac = Vector3(0);

	// Comunes
	float damping;
	float lifeTime;
	Vector4 color;

	struct {
		float vSimulada;
		// no se utiliza en la practica 2
		float invMasa;
	} renderParticle_data;

	struct {
		PxMaterial* material;

		// Como se calcula la masa
		MassDef massDef;
		struct {
			// kg/m^3
			float density;
		} density_data;
		struct {
			Vector3 massDistribution;
		} inertiaTensor_data;
	} rigidBody_data;

	// Geometria
	physx::PxGeometryType::Enum geometry;
	struct {
		Vector3 size;
	} box_data;
	struct {
		float radius;
	} sphere_data;
};
