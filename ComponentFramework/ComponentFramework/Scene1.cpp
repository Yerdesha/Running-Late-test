#include <glew.h>
#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <SDL_opengl.h>
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

Scene1::~Scene1() {
}

bool Scene1::OnCreate() {
	lightSource = Vec3(0.0f, 0.0f, 0.0f);
	projectionMatrix = MMath::perspective(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	viewMatrix = MMath::lookAt(Vec3(0.0f, 10.0f, 30.0f), Vec3(0.0f, 10.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	viewMatrix.print();
	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		printf("SDL ERROR:\n", SDL_GetError());
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		printf("SDL ERROR:\n", Mix_GetError());
	}
	
	if (ObjLoader::loadOBJ("meshes/cube_triangle_export.obj") == false) {
		return false;
	}

	meshPtr = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	shaderPtr = new Shader("shaders/textPhongVert.glsl", "shaders/textPhongFrag.glsl");
	texturePtr = new Texture();
	if (meshPtr == nullptr || shaderPtr == nullptr || texturePtr == nullptr) {
		Debug::FatalError("Couldn't create game object assets", __FILE__, __LINE__);
		return false;
	}

	if (texturePtr->LoadImage("textures/joe-avg.png") == false) {
		Debug::FatalError("Couldn't load texture", __FILE__, __LINE__);
		return false;
	}
	demoObject = new DemoObject(meshPtr, shaderPtr, texturePtr);
	if (demoObject == nullptr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}
	demoObject->setPos(Vec3(0.0, 0.0, 0.0));
	demoObject->setModelMatrix(MMath::translate(demoObject->getPos()));
	demoObject->PhysicsObject::PhysicsObject(1000.0f, 0.0f, 1.0f, 2000.0f);
	return true;
}

void Scene1::HandleEvents(const SDL_Event& sdlEvent) {
	if (sdlEvent.type == SDL_EventType::SDL_KEYDOWN) {
		printf("Key down");
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_W) {
			printf("Pressing W\n");
			demoObject->setVel(Vec3(0.0f, 10.0f, 0.0f));
		}
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_A) {
			printf("Pressing A\n");
			//demoObject->setModelMatrix(MMath::rotate(180, Vec3(0.0f, -1.0f, 0.0f)));
			demoObject->setVel(Vec3(-10.0f, 0.0f, 0.0f));
			
		}
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_S) {
			printf("Pressing S\n");
			demoObject->setVel(Vec3(0.0f, -10.0f, 0.0f));
		}
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_D) {
			printf("Pressing D\n");
			demoObject->setVel(Vec3(10.0f, 0.0f, 0.0f));
		}
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_SPACE) {
			printf("Pressing SPACE\n");
			demoObject->setVel(Vec3(0.0f, 15.0f, 0.0f));
			printf("Sound Playing\n");
			Mix_PlayChannel(-1, jump, 0);
		}
	}
	else if (sdlEvent.type == SDL_EventType::SDL_KEYUP) {
		demoObject->setAccel(Vec3(0.0f, -10.0f, 0.0f));
		demoObject->setVel(Vec3(0.0f, 0.0f, 0.0f));
	}


	sdlEvent1 = sdlEvent;
	
}

void Scene1::Update(const float deltaTime) {
	demoObject->Update(deltaTime);
	HandleEvents(sdlEvent1);
	

	Vec3 demoObjectPos = demoObject->getPos();

	if (demoObject->getPos().y <= -1.0f)
	{
		demoObject->setPos(Vec3(demoObjectPos.x, -1.0f, demoObjectPos.z));
		demoObject->setAccel(Vec3(0.0f, 0.0f, 0.0f));

	}
	
	

	//console output that helps track and tell everything in the console
	if (totalTime2 >= TimeCounter2) {
		TimeCounter2 += deltaTime;
		/*std::cout
			<< "Time: " << (int)totalTime2 << "\t "
			<< "|Position: " << (demoObject->getPos().y) << "\t  "
			<< "|Velocity: " << (demoObject->getVel().y) << "\t  "
			"\n";*/
	}

	jump = Mix_LoadWAV("sounds/jump.wav");

	totalTime2 += deltaTime;
	//demoObject->setVel(Vec3(1.0, 0.0, 0.0));
	//Physics::ApplyForces(*demoObject, demoObject->waterHeight);
	Physics::SimpleNewtonMotion(*demoObject, deltaTime);
	Matrix4 modelMatrix = MMath::translate(demoObject->getPos()) *
		MMath::scale(0.5f, 0.5f, 0.5f);

	demoObject->setModelMatrix(modelMatrix);
	//demoObject->getPos()+=vel * deltaTime + (0.5 * accel * deltaTime)
	
	
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

	Mix_FreeChunk(jump);
	Mix_Quit();
	SDL_Quit();
}
