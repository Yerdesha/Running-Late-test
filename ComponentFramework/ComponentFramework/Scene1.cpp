#include <glew.h>
#include <SDL.h>
#include <iostream>
#include "Debug.h"
#include "Scene1.h"
#include "ObjLoader.h"
#include "ObjLoader2.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "MMath.h"
#include "Physics.h"
#include "DemoObject.h"

float totalTime2 = 0.0f;
float TimeCounter2;

Scene1::Scene1() : meshPtr(nullptr), shaderPtr(nullptr), texturePtr(nullptr) {
	Debug::Info("Created Scene1: ", __FILE__, __LINE__);
}

Scene1::~Scene1() {}

bool Scene1::OnCreate() {
	lightSource = Vec3(0.0f, 0.0f, 0.0f);
	projectionMatrix = MMath::perspective(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	viewMatrix = MMath::lookAt(Vec3(0.0f, 10.0f, 30.0f), Vec3(0.0f, 10.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	viewMatrix.print();

	if (ObjLoader::loadOBJ("meshes/Mario.obj") == false) {
		return false;
	}

	meshPtr = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	shaderPtr = new Shader("shaders/phongVert.glsl", "shaders/phongFrag.glsl");
	texturePtr = new Texture();
	if (meshPtr == nullptr || shaderPtr == nullptr || texturePtr == nullptr) {
		Debug::FatalError("Couldn't create game object assets", __FILE__, __LINE__);
		return false;
	}
	if (texturePtr->LoadImage("textures/viralSurface.jpg") == false) {
		Debug::FatalError("Couldn't load texture", __FILE__, __LINE__);
		return false;
	}
	demoObject = new DemoObject(meshPtr, shaderPtr, texturePtr);
	if (demoObject == nullptr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}
	demoObject->setPos(Vec3(-5.0, 0.0, 0.0));
	demoObject->setModelMatrix(MMath::translate(demoObject->getPos()));
	demoObject->PhysicsObject::PhysicsObject(1000.0f, 1.0f, 1.0f, 2000.0f);
	return true;
}

void Scene1::HandleEvents(const SDL_Event &sdlEvent) {
	
}

void Scene1::Update(const float deltaTime) {
	demoObject->Update(deltaTime);

	//console output that helps track and tell everything in the console
	if (totalTime2 >= TimeCounter2) {
		TimeCounter2 += deltaTime;
		std::cout
			<< "Time: " << (int)totalTime2 << "\t "
			<< "|Position: " << (demoObject->getPos().y) << "\t  "
			<< "|Velocity: " << (demoObject->getVel().y) << "\t  "
			"\n";
	}

	totalTime2 += deltaTime;
	//demoObject->setVel(Vec3(1.0, 0.0, 0.0));
	Physics::ApplyForces(*demoObject, demoObject->waterHeight);
	Physics::SimpleNewtonMotion(*demoObject, deltaTime);
	demoObject->setModelMatrix(MMath::translate(demoObject->getPos()));
	//demoObject->getPos+=vel * deltaTime + (0.5 * accel * deltaTime)
}

void Scene1::Render() const {
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	/// Draw your scene here
	GLuint program = demoObject->getShader()->getProgram();
	glUseProgram(program);

	/// These pass the matricies and the light position to the GPU
	glUniformMatrix4fv(demoObject->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(demoObject->getShader()->getUniformID("viewMatrix"), 1, GL_FALSE, viewMatrix);
	glUniform3fv(demoObject->getShader()->getUniformID("lightPos"), 1, lightSource);

	demoObject->Render();
	glUseProgram(0);
}


void Scene1::OnDestroy() {
	if (meshPtr) delete meshPtr, meshPtr = nullptr;
	if (texturePtr) delete texturePtr, texturePtr = nullptr;
	if (shaderPtr) delete shaderPtr, shaderPtr = nullptr;
	if (demoObject) delete demoObject, demoObject = nullptr;
}
