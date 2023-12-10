#pragma once
#include "SpringForceGenerator.h"

// fuerza entre una particula y una posicion fija
class AnchoredSpringForceGenerator : public SpringForceGenerator {
private:
	physx::PxTransform anchorPose;
	const float HALF_SIZE = 4;
	const Vector4 COLOR = Vector4(0.188, 0.188, 0.188, 1);
	RenderItem* renderItem;

public:
	AnchoredSpringForceGenerator(string name, double k, double restingLength, Vector3 anchoredPos, double duration = -1.0f);

	virtual ~AnchoredSpringForceGenerator();

	virtual inline Vector3 getPosOtherEnd() const {
		return anchorPose.p;
	}

};