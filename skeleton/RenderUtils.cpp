#include <vector>

#include "PxPhysicsAPI.h"

#include "core.hpp"
#include "RenderUtils.hpp"


using namespace physx;

// ARCHIVO QUE SE ENCARGA DEL SISTEMA DE PINTADO
// HAY UN OBJETO DE PINTADO ASOCIADO A CADA OBJETO

extern void initPhysics(bool interactive);
extern void stepPhysics(bool interactive, double t);	
extern void cleanupPhysics(bool interactive);
extern void keyPress(unsigned char key, const PxTransform& camera);
extern PxPhysics* gPhysics;
extern PxMaterial* gMaterial;

// CAMBIO
extern void keyRelease(unsigned char key);

// vector con todos los objetos que quiere renderizar
// cuando se quiera hacer una operación a ese objeto de pintado,
// hay que utilizar esta lista
std::vector<const RenderItem*> gRenderItems;

// CAMBIO
// vector con todos los textos a mostrar
std::vector<DisplayText*> displayTexts;

// CAMBIO
bool click = false;

double PCFreq = 0.0;
__int64 CounterStart = 0;
__int64 CounterLast = 0;

void StartCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		return;

	PCFreq = double(li.QuadPart) /*/ 1000.0*/;

	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
	CounterLast = CounterStart;
}

double GetCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	double t = double(li.QuadPart - CounterLast) / PCFreq;
	CounterLast = li.QuadPart;
	return t;
}

namespace
{
	Camera*	sCamera;

	std::pair<int, int> cursor;

void motionCallback(int x, int y)
{
	sCamera->handleMotion(x, y);
}

// CAMBIO
void keepCursor(int x, int y) {
	cursor.first = x;
	cursor.second = y;
}

void keyboardCallback(unsigned char key, int x, int y)
{
	if(key==27)
		exit(0);

	if(!sCamera->handleKey(key, x, y))
		keyPress(key, sCamera->getTransform());
}

// CAMBIO
void keyboardReleaseCallback(unsigned char key, int x, int y) {
	keyRelease(key);
}

void mouseCallback(int button, int state, int x, int y)
{
	sCamera->handleMouse(button, state, x, y);
	if (state == GLUT_DOWN) {
		click = !click;
	}
}

void idleCallback()
{
	glutPostRedisplay();
}

float stepTime = 0.0f;
//#define FIXED_STEP

void renderCallback()
{
	double t = GetCounter();
#ifdef FIXED_STEP
	if (t < (1.0f / 30.0f))
	{
		fprintf(stderr, "Time: %f\n", stepTime);
		stepTime += t;
	}
	else
		stepTime = 1.0f / 30.0f;

	if (stepTime >= (1.0f / 30.0f))
	{
		stepPhysics(true, stepTime);
		stepTime = 0.0f;
	}
#else
	stepPhysics(true, t);
#endif
	// cámara, se comienza el renderizado
	startRender(sCamera->getEye(), sCamera->getDir());

	//fprintf(stderr, "Num Render Items: %d\n", static_cast<int>(gRenderItems.size()));
	for (auto it = gRenderItems.begin(); it != gRenderItems.end(); ++it)
	{
		const RenderItem* obj = (*it);
		auto objTransform = obj->transform;
		if (!objTransform)
		{
			auto actor = obj->actor;
			if (actor)
			{
				renderShape(*obj->shape, actor->getGlobalPose(), obj->color);
				continue;
			}
		}
		renderShape(*obj->shape, objTransform ? *objTransform : physx::PxTransform(PxIdentity), obj->color);
	}

	//PxScene* scene;
	//PxGetPhysics().getScenes(&scene, 1);
	//PxU32 nbActors = scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
	//if (nbActors)
	//{
	//	std::vector<PxRigidActor*> actors(nbActors);
	//	scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(&actors[0]), nbActors);
	//	renderActors(&actors[0], static_cast<PxU32>(actors.size()), true, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	//}

	for (auto displayT : displayTexts) {
		glColor4f(displayT->getColor().x, displayT->getColor().y, displayT->getColor().z, displayT->getColor().w);
		drawText(displayT->getText(), displayT->getPos().first, displayT->getPos().second, displayT->getFont());
	}

	finishRender();
}

void exitCallback(void)
{
	delete sCamera;
	cleanupPhysics(true);
}
}

void renderLoop()
{
	StartCounter();
	sCamera = new Camera(PxVec3(50.0f, 50.0f, 50.0f), PxVec3(-0.6f,-0.2f,-0.7f));

	setupDefaultWindow("Simulacion Fisica Videojuegos");
	setupDefaultRenderState();

	// CAMBIO
	// este callback se llama cuando se mueve el cursor dentro
	// de la ventana sin pulsar el raton
	glutPassiveMotionFunc(keepCursor);

	glutIdleFunc(idleCallback);
	glutDisplayFunc(renderCallback);
	glutKeyboardFunc(keyboardCallback);

	// CAMBIO
	// se llama cuando se suelta una tecla
	glutKeyboardUpFunc(keyboardReleaseCallback);

	glutMouseFunc(mouseCallback);
	// CAMBIO: se podria comentar
	glutMotionFunc(motionCallback);

	motionCallback(0,0);

	atexit(exitCallback);

	initPhysics(true);
	glutMainLoop();
}

void RegisterRenderItem(const RenderItem* _item)
{
	gRenderItems.push_back(_item);
}

void DeregisterRenderItem(const RenderItem* _item)
{
	auto it = find(gRenderItems.begin(), gRenderItems.end(), _item);
	gRenderItems.erase(it);
}

void RegisterDisplayText(DisplayText* displayText) {
	auto it = find(displayTexts.begin(), displayTexts.end(), displayText);
	if (it == displayTexts.end()) {
		displayTexts.push_back(displayText);
	}
}
void DeregisterDisplayText(DisplayText* displayText) {
	auto it = find(displayTexts.begin(), displayTexts.end(), displayText);
	if (it != displayTexts.end()) {
		displayTexts.erase(it);
	}
}

double GetLastTime()
{
	double t = double(CounterLast - CounterStart) / PCFreq;
	return t;
}

Camera* GetCamera()
{
	return sCamera;
}

// CAMBIO
std::pair<int, int> getCursor() {
	return cursor;
}

// CAMBIO
bool toggleClick() {
	return click;
}

PxShape* CreateShape(const PxGeometry& geo, const PxMaterial* mat)
{
	if (mat == nullptr)
		mat = gMaterial; // Default material

	PxShape* shape = gPhysics->createShape(geo, *mat);
	return shape;
}