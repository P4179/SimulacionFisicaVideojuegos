#pragma once
#include "SpringForceGenerator.h"

// fuerza entre una particula y una posicion fija
class AnchoredSpringForceGenerator : public SpringForceGenerator {
private:
	Vector3 anchoredPos;
	physx::PxTransform anchorPose;
	const float HALF_SIZE = 10;
	const Vector4 COLOR = Vector4(0.188, 0.188, 0.188, 1);
	RenderItem* renderItem;

public:
	AnchoredSpringForceGenerator(string name, double k, double restingLength, Vector3 anchoredPos, double duration = -1.0f) :
		SpringForceGenerator(name, k, restingLength, nullptr, duration), anchorPose(anchoredPos.x, anchoredPos.y, anchoredPos.z) {

		// la caja se inicializa con las mitades de la altura, anchura y profundidad
		physx::PxShape* shape = CreateShape(physx::PxBoxGeometry(HALF_SIZE, 2 * HALF_SIZE, HALF_SIZE));
		// geometría, posición, color (R, G, B, alpha)
		// RGB va desde 0 hasta 1
		// el new ya hace el register
		renderItem = new RenderItem(shape, &anchorPose, COLOR);
	}

	virtual ~AnchoredSpringForceGenerator() {
		DeregisterRenderItem(renderItem);
	}

	virtual inline Vector3 getPosOtherEnd() const {
		return anchoredPos;
	}

};