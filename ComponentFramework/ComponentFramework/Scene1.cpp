#include <glew.h>
#include <SDL.h>
#include "Debug.h"
#include "Scene1.h"

#include "ObjLoader.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "MMath.h"
#include "Physics.h"

Scene1::Scene1() : meshPtr(nullptr), shaderPtr(nullptr), texturePtr(nullptr) {
	Debug::Info("Created Scene1: ", __FILE__, __LINE__);
}

Scene1::~Scene1() {}

bool Scene1::OnCreate() {
	
	return true;
}

void Scene1::HandleEvents(const SDL_Event &sdlEvent) {
	
}

void Scene1::Update(const float deltaTime) {
	
	
}

void Scene1::Render() const {
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	/// Draw your scene here
	

	glUseProgram(0);
}


void Scene1::OnDestroy() {
	
}
