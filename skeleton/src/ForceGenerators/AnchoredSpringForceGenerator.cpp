#include "AnchoredSpringForceGenerator.h"
#include "../checkML.h"

AnchoredSpringForceGenerator::AnchoredSpringForceGenerator(string name, double k, double restingLength, Vector3 anchoredPos, double duration) :
	SpringForceGenerator(name, k, restingLength, nullptr, duration), anchorPose(anchoredPos.x, anchoredPos.y, anchoredPos.z) {

	// la caja se inicializa con las mitades de la altura, anchura y profundidad
	physx::PxShape* shape = CreateShape(physx::PxBoxGeometry(HALF_SIZE, HALF_SIZE, HALF_SIZE));
	// geometría, posición, color (R, G, B, alpha)
	// RGB va desde 0 hasta 1
	// el new ya hace el register
	renderItem = new RenderItem(shape, &anchorPose, COLOR);
}

AnchoredSpringForceGenerator::~AnchoredSpringForceGenerator() {
	DeregisterRenderItem(renderItem);
}